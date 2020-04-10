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

#include "genetic_algorithm.h"
#include "genetic_algorithm_crossovers.h"

#include "utils.h"

/* Prototypes */

/* globals */
short global_index;

/*---------------------------------------------------------------------------------------------
 * (function: ga_init)
 *-------------------------------------------------------------------------------------------*/
void ga_init(population_t **(*fptr_init_starting_population)(void))
{
	/* intialize population - there's two so we can toggle between two */
	genomes.population = (population_t ***)malloc(sizeof(population_t**)*2);
	genomes.population[0] = (*fptr_init_starting_population)();
	genomes.population[1] = (*fptr_init_starting_population)();
	genomes.costs = (double*)malloc(sizeof(double)*genomes.num_population);
}

/*---------------------------------------------------------------------------------------------
 * (function: ga_clean)
 *-------------------------------------------------------------------------------------------*/
void ga_clean(void (*fptr_clean_population)(void))
{
	(*fptr_clean_population)();
	free(genomes.population);
}
/*---------------------------------------------------------------------------------------------
 * (function: run_ga)
 *-------------------------------------------------------------------------------------------*/
int run_base_ga(
		int (*fptr_selector)(),
		void (*fptr_selector_init)(int),
		void (*fptr_cost_function_order)(double (*)(void *)),
		double (*fptr_cost_function)(void *),
		void (*fptr_copy_old_over)(population_t**, population_t**, int, int),
		void (*fptr_cross_breed)(
					void (*)(void *, void *, void *, int), 
					int (*)(),
					void (*)(int),
					population_t**, population_t**, int, int),
		void (*fptr_mutate)(population_t**, population_t**, int, int, int),
		void (*fptr_mutate_no_copy)(population_t**, population_t**, int, int, int),
		void (*fptr_random_new)(population_t**, int, int),
		void (*fptr_cost_report_best)(FILE *, double (*)(void *)),
		void (*fptr_crossover)(void *, void *, void *, int),
		short (*fptr_exit_condition)(void)
		)
{
	short exit = FALSE;
	short to_pop_idx;

	double time_start_wall = 0;
	double time_end_wall = 0;

	double time_start_mutation = 0;
	double time_end_mutation = 0;
	double time_total_mutation = 0;

	double time_start_crossbreed = 0;
	double time_end_crossbreed = 0;
	double time_total_crossbreed = 0;

	double time_start_cost_function = 0;
	double time_end_cost_function = 0;
	double time_total_cost_function = 0;

	double time_start_random_new = 0;
	double time_end_random_new = 0;
	double time_total_random_new = 0;

	double time_start_crossbreed_mutation = 0;
	double time_end_crossbreed_mutation = 0;
	double time_total_crossbreed_mutation = 0;

	double time_start_keep = 0;
	double time_end_keep = 0;
	double time_total_keep = 0;

	global_index = 0;

	/* calculate indexes for new population */
	int idx_start_keep = 0;
	int idx_end_keep = (int)floor(genomes.num_population * genomes.percent_to_keep);
	int idx_start_mutate = idx_end_keep;
	int idx_end_mutate = idx_end_keep + (int)(floor(genomes.num_population * genomes.percent_from_mutate));
	int idx_start_breed = idx_end_mutate;
	int idx_end_breed = idx_end_mutate + (int)(floor(genomes.num_population * genomes.percent_from_breed));
	int idx_start_breed_and_mutate = idx_end_breed;
	int idx_end_breed_and_mutate = idx_end_breed + (int)(floor(genomes.num_population * genomes.percent_from_breed_and_mutate));
	int idx_start_random = idx_end_breed_and_mutate;
	int idx_end_random = idx_end_breed_and_mutate + (int)(floor(genomes.num_population * genomes.percent_to_random));

	int idx_end_breed_from = (int)(floor(genomes.num_population * genomes.top_percent_to_breed_from));
	int idx_end_mutate_from = (int)(floor(genomes.num_population * genomes.top_percent_to_mutate_from));
	int idx_end_breed_mutate_from = (int)(floor(genomes.num_population * genomes.top_percent_to_breed_mutate_from));

	printf("idx_start_keep = %d\n", idx_start_keep);
	printf("idx_end_keep  = %d\n", idx_end_keep );
	printf("idx_start_mutate = %d\n", idx_start_mutate);
	printf("idx_end_mutate = %d\n", idx_end_mutate);
	printf("idx_start_breed = %d\n", idx_start_breed);
	printf("idx_end_breed = %d\n", idx_end_breed);
	printf("idx_start_breed_and_mutate = %d\n", idx_start_breed_and_mutate);
	printf("idx_end_breed_and_mutate = %d\n", idx_end_breed_and_mutate);
	printf("idx_start_random = %d\n", idx_start_random);
	printf("idx_end_random = %d\n", idx_end_random);
	printf("idx_end_breed_from	= %d\n", idx_end_breed_from );
	printf("idx_end_mutate_from = %d\n", idx_end_mutate_from);
	printf("idx_end_breed_mutate_from = %d\n", idx_end_breed_mutate_from);

	time_start_wall = get_wall_time();

	while (exit == FALSE)
	{
		to_pop_idx = (global_index+1)%2;

		/* TIMER */
		time_start_cost_function = get_wall_time();
		/* EVALUATE POPULATION AND ORDER */
		(*fptr_cost_function_order)(
						(*fptr_cost_function));

		time_end_cost_function = get_wall_time();
		time_total_cost_function += time_end_cost_function - time_start_cost_function;

		/* TIMER */
		time_start_keep = get_wall_time();
		/* KEEP OLD */
		(*fptr_copy_old_over)(genomes.population[global_index], genomes.population[to_pop_idx], idx_start_keep, idx_end_keep);
		time_end_keep = get_wall_time();
		time_total_keep += time_end_keep - time_start_keep;

		/* TIMER */
		time_start_crossbreed = get_wall_time();
		/* CROSSBREED NEW */
		(*fptr_cross_breed)(
				(*fptr_crossover),
				(*fptr_selector),
				(*fptr_selector_init),
				genomes.population[global_index], 
				genomes.population[to_pop_idx], 
				idx_start_breed, 
				idx_end_breed);
		time_end_crossbreed = get_wall_time();
		time_total_crossbreed += time_end_crossbreed - time_start_crossbreed;

		/* TIMER */
		time_start_mutation = get_wall_time();
		/* MUTATE NEW */
		(*fptr_mutate)(genomes.population[global_index], genomes.population[to_pop_idx], idx_start_mutate, idx_end_mutate, idx_end_mutate_from);
		time_end_mutation = get_wall_time();
		time_total_mutation += time_end_mutation - time_start_mutation;

		/* TIMER */
		time_start_crossbreed_mutation = get_wall_time();
		/* CROSSBREED & MUTATE NEW */
		/* TIMER */
		time_start_crossbreed = get_wall_time();
		(*fptr_cross_breed)(
				(*fptr_crossover),
				(*fptr_selector),
				(*fptr_selector_init),
				genomes.population[global_index], 
				genomes.population[to_pop_idx], 
				idx_start_breed_and_mutate, 
				idx_end_breed_and_mutate);
		time_end_crossbreed = get_wall_time();
		time_total_crossbreed += time_end_crossbreed - time_start_crossbreed;

		/* TIMER */
		time_start_mutation = get_wall_time();
		/* MUTATE NEW */
		(*fptr_mutate_no_copy)(genomes.population[global_index], genomes.population[to_pop_idx], idx_start_breed_and_mutate, idx_end_breed_and_mutate, idx_end_mutate_from);
		time_end_mutation = get_wall_time();
		time_total_mutation += time_end_mutation - time_start_mutation;
		time_end_crossbreed_mutation = get_wall_time();
		time_total_crossbreed_mutation += time_end_crossbreed_mutation - time_start_crossbreed_mutation;

		/* TIMER */
		time_start_random_new = get_wall_time();
		/* CREATE RANDOM NEW */
		(*fptr_random_new)(genomes.population[to_pop_idx], idx_start_random, idx_end_random);
		time_end_random_new = get_wall_time();
		time_total_random_new += time_end_random_new - time_start_random_new;

		/* CHECK EXIT CONDITION */
		exit = (*fptr_exit_condition)();

#if 0
		(*fptr_cost_report_best)(
				fresult_out, 
				(*fptr_cost_function));
#endif

		/* swap to the new population */
		global_index = to_pop_idx;
	}

	time_end_wall = get_wall_time();
#ifdef TIMING_RESULTS
	fprintf(ftest_out, "%s, Wall_clock_time , %lf\n", (char*)global_args.config_file, time_end_wall - time_start_wall);
	fprintf(ftest_out, "%s, Wall_clock_time_mutation , %lf\n", (char*)global_args.config_file, time_total_mutation);
	fprintf(ftest_out, "%s, Wall_clock_time_crossbreed , %lf\n", (char*)global_args.config_file, time_total_crossbreed);
	fprintf(ftest_out, "%s, Wall_clock_time_crossbreed+mutation , %lf\n", (char*)global_args.config_file, time_total_crossbreed_mutation);
	fprintf(ftest_out, "%s, Wall_clock_time_random_new , %lf\n", (char*)global_args.config_file, time_total_random_new);
	fprintf(ftest_out, "%s, Wall_clock_time_cost_function , %lf\n", (char*)global_args.config_file, time_total_cost_function);
	fprintf(ftest_out, "%s, Wall_clock_time_keep , %lf\n", (char*)global_args.config_file, time_total_keep);
#endif
}

/*---------------------------------------------------------------------------------------------
 * (function: output_test_details)
 *-------------------------------------------------------------------------------------------*/
void output_test_details(double final_cost)
{
	fprintf(ftest_out, "%s, Best_cost, %f\n", (char*)global_args.config_file, final_cost);
}
