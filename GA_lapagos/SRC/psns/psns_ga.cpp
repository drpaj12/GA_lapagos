/*
Copyright (c) 2019 Peter Andrew Jamieson (jamieson.peter@gmail.com)

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/ 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "globals.h"
#include "types.h"
#include "utils.h"

#include "genetic_algorithm.h"
#include "genetic_algorithm_combinational_crossovers.h"
#include "genetic_algorithm_selectors.h"

#include "psns_globals.h"
#include "psns_types.h"

#include "psns_ga.h"
#include "psns_ga_genome_binary.h"
#include "psns_dat.h"
#include "psns_kmeans.h"

/* Prototypes */

psns_problem_t psns_problem;
psns_problem_bits_t psns_genome_bits;

const char *psns_problem_types_name[] = {    "PSNS_DAT_FORMAT_BITS",
                                            "PSNS_DAT_FORMAT_GRAY_CODE" 
                                       };

/*-------------------------------------------------------------------------
 * (function: psns_setup_problem)
 *-----------------------------------------------------------------------*/
void psns_setup_problem()
{
    psns_problem.problem_type = (enum PSNS_PROBLEM_TYPES)return_string_in_list(configuration.problem_type, (char**)psns_problem_types_name, NUM_PSNS_PROBLEM_TYPES);

    switch(psns_problem.problem_type)
    {
        case PSNS_DAT_FORMAT_BITS:
        case PSNS_DAT_FORMAT_GRAY_CODE:
	        /* read the benchmark xml in */
           	read_psns_problem_dat(configuration.benchmark_file_name);
            break;
        default:
            printf("Not recognized psns problem type!!!\n");
    }
}

/*-------------------------------------------------------------------------
 * (function: psns_free_problem)
 *-----------------------------------------------------------------------*/
void psns_free_problem()
{
    switch(psns_problem.problem_type)
    {
        case PSNS_DAT_FORMAT_BITS:
        case PSNS_DAT_FORMAT_GRAY_CODE:
			/* free data structures */
			free_psns_problem_dat();
            break;
        default:
            printf("Not recognized psns problem type!!!\n");
    }
}

/*-------------------------------------------------------------------------
 * (function: psns_do_in_ga_init)
 *-----------------------------------------------------------------------*/
void psns_do_in_ga_init()
{
    switch(psns_problem.problem_type)
    {
        case PSNS_DAT_FORMAT_BITS:
        case PSNS_DAT_FORMAT_GRAY_CODE:
	        ga_init(psns_create_population_bits);
            break;
            break;
        default:
            printf("Not recognized psns problem type!!!\n");
    }
}

/*-------------------------------------------------------------------------
 * (function: psns_do_in_ga_clean)
 *-----------------------------------------------------------------------*/
void psns_do_in_ga_clean()
{
    switch(psns_problem.problem_type)
    {
        case PSNS_DAT_FORMAT_BITS:
        case PSNS_DAT_FORMAT_GRAY_CODE:
    	    ga_clean(psns_free_population_bits);
            break;
        default:
            printf("Not recognized psns problem type!!!\n");
    }
}

/*-------------------------------------------------------------------------
 * (function: psns_run_ga)
 *-----------------------------------------------------------------------*/
void psns_run_ga()
{
    double (*fptr_cost_function)(void *);
       
    switch(psns_problem.problem_type)
    {
        case PSNS_DAT_FORMAT_BITS:
            fptr_cost_function = psns_cost_function_bits;

		    run_base_ga(
		        (int (*)())setup_selector_function(),
		        (void (*)(int))setup_selector_init_function(),
		        psns_cost_function_and_order,
		        fptr_cost_function,
		        psns_copy_old_genomes_bits,
		        psns_cross_breed_bits,
		        psns_mutate_bits,
		        psns_breed_and_mutate_bits,
		        psns_random_new_bits,
		        psns_cost_report_best,
		        (void (*)(void*,void*,void*,void*,int))setup_crossover_function(),
		        psns_exit_condition);
            break;
        case PSNS_DAT_FORMAT_GRAY_CODE:
            fptr_cost_function = psns_cost_function_gray_code_bits;

		    run_base_ga(
		        (int (*)())setup_selector_function(),
		        (void (*)(int))setup_selector_init_function(),
		        psns_cost_function_and_order,
		        fptr_cost_function,
		        psns_copy_old_genomes_bits,
		        psns_cross_breed_bits,
		        psns_mutate_bits,
		        psns_breed_and_mutate_bits,
		        psns_random_new_bits,
		        psns_cost_report_best,
		        (void (*)(void*,void*,void*,void*,int))setup_crossover_function(),
		        psns_exit_condition);
            break;
        default:
            printf("Not recognized psns problem type!!!\n");
            return;
    }

    /* TEST OUT REPORT */
    output_test_details((*fptr_cost_function)(genomes.population[global_index][0]->genome));

    /* OTHER methods */
    psns_solve_by_kmeans();
    printf("Cost after kmeans %f\n", psns_cost_function_after_assignment());
    psns_solve_by_weighted_kmeans();
    printf("Cost after weighted kmeans %f\n", psns_cost_function_after_assignment());
    psns_solve_by_kmeans_with_time();
    printf("Cost after kmeans with time %f\n", psns_cost_function_after_assignment());
    psns_solve_by_weighted_kmeans_with_time();
    printf("Cost after weighted kmeans with time %f\n", psns_cost_function_after_assignment());
}

/*---------------------------------------------------------------------------------------------
 * (function: psns_exit_condition)
 *-------------------------------------------------------------------------------------------*/
short psns_exit_condition()
{
	static int breeding_cycles = 0;
	static int count = 0;
	static float recuring_best = 0;

	breeding_cycles ++;
	printf("breed cycle: %d:", breeding_cycles);
	fprintf(fresult_out, "%d", breeding_cycles);
	fflush(stdout);

	if (global_best > recuring_best)
	{
		recuring_best = global_best;
		count = 0;
	}
	else
	{
		count ++;
	}

	if (count > 50)
	{
		count = 0;
		breeding_cycles = 0;
		return TRUE; // exit
	}
	else
	{
		return FALSE;
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: psns_cost_report_best)
 *-------------------------------------------------------------------------------------------*/
void psns_cost_report_best(
        FILE *fout, 
		double (*fptr_cost_function)(void *)
        )
{
	float *costs;
	int i, j;
	int index_of_greatest_cost;

	costs = (float*)malloc(sizeof(float)*genomes.num_population);
	for (i = 0; i < genomes.num_population; i++)
	{
		costs[i] = (*fptr_cost_function)(genomes.population[global_index][i]->genome);
	}

	/* Sort the list of populations */
	for (i = 0; i < genomes.num_population - 1; i++)
	{
		index_of_greatest_cost = i;
		population_t *temp_pop;
		float temp_cost;	
		for (j = i + 1; j < genomes.num_population; j ++)
		{
			if (costs[index_of_greatest_cost] < costs[j])
				index_of_greatest_cost = j;
		}

		/* swap the top with new low */
		temp_pop = genomes.population[global_index][i];
		temp_cost = costs[i];

		genomes.population[global_index][i] = genomes.population[global_index][index_of_greatest_cost];
		costs[i] = costs[index_of_greatest_cost];

		genomes.population[global_index][index_of_greatest_cost] = temp_pop;
		costs[index_of_greatest_cost] = temp_cost;
	}

	if (fout != NULL)
	{
		fprintf(fout, "\t%f\n", costs[0]);
	}
	else
	{
		printf("Best solution costs: %f\n", costs[0]);
	}
	global_best = costs[0];
	free(costs);
}

/*---------------------------------------------------------------------------------------------
 * (function: psns_cost_function_and_order)
 *-------------------------------------------------------------------------------------------*/
void psns_cost_function_and_order(
		double (*fptr_cost_function)(void *)
        )
{
	int i, j;
	int index_of_greatest_cost;

	for (i = 0; i < genomes.num_population; i++)
	{
		genomes.costs[i] = (*fptr_cost_function)(genomes.population[global_index][i]->genome);
	}

	/* Sort the list of populations */
	for (i = 0; i < genomes.num_population - 1; i++)
	{
		index_of_greatest_cost = i;
		population_t *temp_pop;
		float temp_cost;	
		for (j = i + 1; j < genomes.num_population; j ++)
		{
			if (genomes.costs[index_of_greatest_cost] < genomes.costs[j])
				index_of_greatest_cost = j;
		}

		/* swap the top with new low */
		temp_pop = genomes.population[global_index][i];
		temp_cost = genomes.costs[i];

		genomes.population[global_index][i] = genomes.population[global_index][index_of_greatest_cost];
		genomes.costs[i] = genomes.costs[index_of_greatest_cost];

		genomes.population[global_index][index_of_greatest_cost] = temp_pop;
		genomes.costs[index_of_greatest_cost] = temp_cost;
	}

	printf("Best solution costs: %f\n", genomes.costs[0]);
	global_best = genomes.costs[0];
}
