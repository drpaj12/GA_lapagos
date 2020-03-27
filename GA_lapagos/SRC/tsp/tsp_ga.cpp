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

#include "tsp_globals.h"
#include "tsp_types.h"

#include "tsp_ga_genome_combinatorial_tupple.h"
#include "tsp_ga_genome_combinatorial_tupple_lehmer.h"
#include "tsp_ga_genome_combinatorial_tupple_random_keys.h"
#include "tsp_adjacency.h"
#include "tsp_cartesian.h"
#include "tsp_ga_lehmer_encoding.h"
#include "tsp_ga_random_key_encoding.h"

/* Prototypes */

tsp_problem_t tsp_problem;
const char *tsp_problem_types_name[] = { "ADJACENCY_PERMUTATION", 
					"ADJACENCY_LEHMER", 
					"ADJACENCY_RANDOM_KEYS", 
					"CARTESIAN" };

/*-------------------------------------------------------------------------
 * (function: tsp_setup_problem)
 *-----------------------------------------------------------------------*/
void tsp_setup_problem()
{

	tsp_problem.problem_type = (enum TSP_PROBLEM_TYPES)return_string_in_list(configuration.problem_type, (char**)tsp_problem_types_name, NUM_TSP_PROBLEM_TYPES);

	switch(tsp_problem.problem_type)
	{
		case ADJACENCY_PERMUTATION:
			/* read the benchmark xml in */
			read_tsp_problem_adjacency(configuration.benchmark_file_name, &tsp_problem_adjacency);
			break;
		case ADJACENCY_LEHMER:
			/* read the benchmark xml in */
			read_tsp_problem_adjacency(configuration.benchmark_file_name, &tsp_problem_adjacency);
			lehmer_initialize_pop_list(tsp_problem.num_cities);
			break;
		case ADJACENCY_RANDOM_KEYS:
			/* read the benchmark xml in */
			read_tsp_problem_adjacency(configuration.benchmark_file_name, &tsp_problem_adjacency);
			random_key_quick_sort_init_list(tsp_problem.num_cities);
			break;
		case CARTESIAN:
			/* initialize a random city */
			tsp_init_problem_cartesian(tsp_problem.num_cities);
			break;
		default:
			printf("Not recognized tsp problem type!!!\n");
	}
}

/*-------------------------------------------------------------------------
 * (function: tsp_free_problem)
 *-----------------------------------------------------------------------*/
void tsp_free_problem()
{
	switch(tsp_problem.problem_type)
	{
		case ADJACENCY_PERMUTATION:
			/* free data structures */
			free_tsp_problem_adjacency(tsp_problem_adjacency);
			break;
		case ADJACENCY_LEHMER:
			/* free data structures */
			free_tsp_problem_adjacency(tsp_problem_adjacency);
			lehmer_clean(tsp_problem.num_cities);
			break;
		case ADJACENCY_RANDOM_KEYS:
			/* free data structures */
			free_tsp_problem_adjacency(tsp_problem_adjacency);
			random_key_quick_sort_clean_list();
			break;
		case CARTESIAN:
			/* initialize a random city */
			tsp_free_problem_cartesian();
			break;
		default:
			printf("Not recognized tsp problem type!!!\n");
	}
}

/*-------------------------------------------------------------------------
 * (function: tsp_do_in_ga_init)
 *-----------------------------------------------------------------------*/
void tsp_do_in_ga_init()
{
	switch(tsp_problem.problem_type)
	{
		case ADJACENCY_LEHMER:
			ga_init(tsp_create_population_lehmer);
			break;
		case ADJACENCY_RANDOM_KEYS:
			ga_init(tsp_create_population_random_keys);
			break;
		case ADJACENCY_PERMUTATION:
		case CARTESIAN:
			ga_init(tsp_create_population_permutation);
			break;
		default:
			printf("Not recognized tsp problem type!!!\n");
			return;
	}
}

/*-------------------------------------------------------------------------
 * (function: psns_do_in_ga_clean)
 *-----------------------------------------------------------------------*/
void tsp_do_in_ga_clean()
{
	switch(tsp_problem.problem_type)
	{
		case ADJACENCY_LEHMER:
			ga_clean(tsp_free_population);
			break;
		case ADJACENCY_RANDOM_KEYS:
			ga_clean(tsp_free_population);
			break;
		case ADJACENCY_PERMUTATION:
		case CARTESIAN:
			ga_clean(tsp_free_population);
			break;
		default:
			printf("Not recognized tsp problem type!!!\n");
			return;
	}
}


/*-------------------------------------------------------------------------
 * (function: tsp_run_ga)
 *-----------------------------------------------------------------------*/
void tsp_run_ga()
{
	double (*fptr_cost_function)(void *);
	void (*fptr_random_new)(population_t **, int , int );
	void (*fptr_mutate)(population_t **, population_t **, int , int , int );
	void (*fptr_breed_and_mutate)(
			void (*fptr_crossover)(void *, void *, void *, void *, int),
			int (*fptr_selector)(),
			void (*fptr_selector_init)(int),
			population_t **from, 
			population_t **to, 
			int start, 
			int end);


	switch(tsp_problem.problem_type)
	{
		case ADJACENCY_PERMUTATION:
			fptr_cost_function = tsp_cost_function_from_adjacency_permutation;
			fptr_random_new = tsp_random_new;
			fptr_mutate = tsp_mutate;
			fptr_breed_and_mutate = tsp_breed_and_mutate;
			break;
		case CARTESIAN:
			fptr_cost_function = tsp_cost_function_from_cartesian;
			fptr_random_new = tsp_random_new;
			fptr_mutate = tsp_mutate;
			fptr_breed_and_mutate = tsp_breed_and_mutate;
			break;
		case ADJACENCY_LEHMER:
			fptr_cost_function = tsp_cost_function_from_adjacency_lehmer;
			fptr_random_new = tsp_random_new_lehmer;
			fptr_mutate = tsp_mutate_lehmer;
			fptr_breed_and_mutate = tsp_breed_and_mutate_lehmer;
			break;
		case ADJACENCY_RANDOM_KEYS:
			fptr_cost_function = tsp_cost_function_from_adjacency_random_keys;
			fptr_random_new = tsp_random_new_random_keys;
			fptr_mutate = tsp_mutate_random_keys;
			fptr_breed_and_mutate = tsp_breed_and_mutate_random_keys;
			break;

		default:
			printf("Not recognized tsp problem type!!!\n");
			return;
	}

	run_base_ga(
			(int (*)())setup_selector_function(),
			(void (*)(int))setup_selector_init_function(),
			tsp_cost_function_and_order,
			fptr_cost_function,
			tsp_copy_old_genomes,
			tsp_cross_breed,
			fptr_mutate,
			fptr_breed_and_mutate,
			fptr_random_new,
			tsp_cost_report_best,
			(void (*)(void*,void*,void*,void*,int))setup_crossover_function(),
			tsp_exit_condition);

	/* TEST OUT REPORT */
	output_test_details((*fptr_cost_function)(genomes.population[global_index][0]->genome));
}
