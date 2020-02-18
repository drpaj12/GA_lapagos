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
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <math.h>
#include "globals.h"
#include "types.h"
#include "utils.h"

#include "mkp_globals.h"
#include "mkp_types.h"
#include "mkp_dat.h"

#include "mkp_ga_genome_binary.h"

#include "genetic_algorithm_combinational_crossovers.h"

/*---------------------------------------------------------------------------------------------
 * (function: mkp_create_population)
 *-------------------------------------------------------------------------------------------*/
population_t **mkp_create_population()
{
	population_t **population;
	int i;

	population = (population_t **)malloc(sizeof(population_t*) * genomes.num_population);
	for (i = 0; i < genomes.num_population; i++)
	{
		population[i] = (population_t *)malloc(sizeof(population_t));
		population[i]->genome = (void*)mkp_create_random_solution();
	}

	return population;
}

/*---------------------------------------------------------------------------------------------
 * (function: mkp_free_population)
 *-------------------------------------------------------------------------------------------*/
void mkp_free_population()
{
	int i;

	for (i = 0; i < genomes.num_population; i++)
	{
        bitstr_del((bstr)genomes.population[0][i]->genome);
		free(genomes.population[0][i]);
        bitstr_del((bstr)genomes.population[1][i]->genome);
		free(genomes.population[1][i]);
	}
	free(genomes.population[0]);
	free(genomes.population[1]);
}

/*---------------------------------------------------------------------------------------------
 * (function: mkp_cost_function_and_order)
 *-------------------------------------------------------------------------------------------*/
void mkp_cost_function_and_order(
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
//    bitstr_print((bstr)genomes.population[global_index][0]->genome);
    mkp_verify_satisfiability((bstr)genomes.population[global_index][0]->genome);
}

/*---------------------------------------------------------------------------------------------
 * (function: mkp_cost_report_best)
 *-------------------------------------------------------------------------------------------*/
void mkp_cost_report_best(
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
 * (function: mkp_copy_old_genomes)
 *-------------------------------------------------------------------------------------------*/
void mkp_copy_old_genomes(population_t **from, population_t **to, int start, int end)
{
	int i;

	for (i = start; i < end; i++)
	{
		mkp_copy_solution((bstr)from[i]->genome, (bstr)to[i]->genome);
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: mkp_copy_solution)
 *-------------------------------------------------------------------------------------------*/
void mkp_copy_solution(bstr from, bstr to)
{
	int i;

    bitstr_copy(from, to);
}

/*---------------------------------------------------------------------------------------------
 * (function: mkp_cross_breed)
 *-------------------------------------------------------------------------------------------*/
void mkp_cross_breed(
                            void (*fptr_crossover)(void *, void *, void *, void *, int),
                            int (*fptr_selector)(),
                            void (*fptr_selector_init)(int),
                            population_t **from, 
                            population_t **to, 
                            int start, 
                            int end)
{
	int i, j;
	int parent1;
	int parent2;
	int *genome_p1;
	int *genome_p2;
	int *genome_c1;
	int *genome_c2;

	if (start == end)
		return;

    /* initialize the selector */
    (*fptr_selector_init)(end-start);

	int temp_end = end-(end - start)%2; // make sure even number
	for (i = start; i < temp_end; i += 2)
	{
        /* pick up 2 parents */
		parent1 = (*fptr_selector)();
		parent2 = (*fptr_selector)();

		if (parent1 == parent2)
		{
			/* if two random numbers are the same */
			parent2 = (parent2 != genomes.num_population-1) ? parent2 + 1 : parent2 - 1;
		}

        /* setup parents and child */
		genome_p1 = (int*)from[parent1]->genome;
		genome_p2 = (int*)from[parent2]->genome;
		genome_c1 = (int*)to[i]->genome;
		genome_c2 = (int*)to[i+1]->genome;

		(*fptr_crossover)(genome_p1, genome_p2, genome_c1, genome_c2, mkp_problem.num_variables);
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: mkp_mutate)
 *-------------------------------------------------------------------------------------------*/
void mkp_mutate(population_t **from, population_t **to, int start, int end, int from_best)
{
	int i, j;
	int who;
	int num_mutations = (int)floor(genomes.percent_of_genome_mutations * mkp_problem.num_variables);
	int idx_to_mutate;

	for (i = start; i < end; i++)
	{
		/* find out who is the parent, copy over to "to" */
		who = rand() % from_best;
		mkp_copy_solution((bstr)from[who]->genome, (bstr)to[i]->genome);

		/* mutate the copied genome */ 
		for (j = 0; j < num_mutations; j++)
		{
			idx_to_mutate = rand() % mkp_problem.num_variables;
            bitstr_flip((bstr)to[i]->genome, idx_to_mutate);
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: mkp_breed_and_mutate)
 *-------------------------------------------------------------------------------------------*/
void mkp_breed_and_mutate(
                            void (*fptr_crossover)(void *, void *, void *, void *, int),
                            int (*fptr_selector)(),
                            void (*fptr_selector_init)(int),
                            population_t **from, 
                            population_t **to, 
                            int start, 
                            int end)
{
	int i, j;
	int num_mutations = (int)floor(genomes.percent_of_genome_mutations * mkp_problem.num_variables);
	int idx_to_mutate;

	/* do the cross breeding */
	mkp_cross_breed(fptr_crossover, fptr_selector, fptr_selector_init, from, to, start, end);

	/* mutate */
	for (i = start; i < end; i++)
	{
		/* mutate the copied genome */ 
		for (j = 0; j < num_mutations; j++)
		{
			idx_to_mutate = rand() % mkp_problem.num_variables;
            bitstr_flip((bstr)to[i]->genome, idx_to_mutate);
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: mkp_exit_condition)
 *-------------------------------------------------------------------------------------------*/
short mkp_exit_condition()
{
	static int breeding_cycles = 0;
	static int count = 0;
	static float recuring_best = 0;

	breeding_cycles ++;
	printf("breed cycle: %d\n", breeding_cycles);
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

	if (count > 500)
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
 * (function: mkp_create_random_solution)
 *-------------------------------------------------------------------------------------------*/
void mkp_random_new(population_t **to, int start, int end)
{
	int i, j;
	/* do num cities */
	int temp;

	for (i = start; i < end; i++)
	{
	    for (j = 0; j < mkp_problem.num_variables; j++)
		{
	        if (rand()%2 == 0)
	            bitstr_clear((bstr)to[i]->genome, j);
	        else
	            bitstr_set((bstr)to[i]->genome, j);
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: mkp_create_random_solution)
 *-------------------------------------------------------------------------------------------*/
void *mkp_create_random_solution()
{
	bstr return_genome;
	int i;

	return_genome = bitstr_new(mkp_problem.num_variables);

	for (i = 0; i < mkp_problem.num_variables; i++)
	{
        if (rand()%2 == 0)
            bitstr_clear(return_genome, i);
        else
            bitstr_set(return_genome, i);
	}

	return return_genome;
}
