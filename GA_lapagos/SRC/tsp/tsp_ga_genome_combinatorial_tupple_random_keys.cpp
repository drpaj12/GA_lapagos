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

#include "tsp_globals.h"
#include "tsp_types.h"

#include "tsp_ga_genome_combinatorial_tupple.h"
#include "tsp_ga_genome_combinatorial_tupple_random_keys.h"

#include "genetic_algorithm_combinational_crossovers.h"

/*---------------------------------------------------------------------------------------------
 * (function: tsp_create_population_random_keys)
 *-------------------------------------------------------------------------------------------*/
population_t **tsp_create_population_random_keys()
{
	population_t **population;
	int i;

	population = (population_t **)malloc(sizeof(population_t*) * genomes.num_population);
	for (i = 0; i < genomes.num_population; i++)
	{
		population[i] = (population_t *)malloc(sizeof(population_t));
		population[i]->genome = (void*)tsp_create_random_solution_random_keys();
	}

	return population;
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_mutate_random_keys)
 * These are uniform mutations - meaning this is a random number in range
 *-------------------------------------------------------------------------------------------*/
void tsp_mutate_random_keys(population_t **from, population_t **to, int start, int end, int from_best)
{
	int i, j;
	int who;
	int num_mutations = (int)floor(genomes.percent_of_genome_mutations * tsp_problem.num_cities);
	int location_idx;

	for (i = start; i < end; i++)
	{
		/* find out who is the parent, copy over to "to" */
		who = rand() % from_best;
		tsp_copy_solution((int*)from[who]->genome, (int*)to[i]->genome);

		/* mutate the copied genome */ 
		for (j = 0; j < num_mutations; j++)
		{
			location_idx = rand() % tsp_problem.num_cities;
			((int*)(to[i]->genome))[location_idx] = rand();
		}
	}
}
/*---------------------------------------------------------------------------------------------
 * (function: tsp_mutate_random_keys)
 * These are uniform mutations - meaning this is a random number in range
 *-------------------------------------------------------------------------------------------*/
void tsp_mutate_no_copy_random_keys(population_t **from, population_t **to, int start, int end, int from_best)
{
	int i, j;
	int who;
	int num_mutations = (int)floor(genomes.percent_of_genome_mutations * tsp_problem.num_cities);
	int location_idx;

	for (i = start; i < end; i++)
	{
		/* mutate the copied genome */ 
		for (j = 0; j < num_mutations; j++)
		{
			location_idx = rand() % tsp_problem.num_cities;
			((int*)(to[i]->genome))[location_idx] = rand();
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_breed_and_mutate_random_keys)
 * These are uniform mutations
 *-------------------------------------------------------------------------------------------*/
void tsp_breed_and_mutate_random_keys(
			void (*fptr_crossover)(void *, void *, void *, int),
			int (*fptr_selector)(),
			void (*fptr_selector_init)(int),
			population_t **from, 
			population_t **to, 
			int start, 
			int end)
{
	int i, j;
	int who;
	int num_mutations = (int)floor(genomes.percent_of_genome_mutations * tsp_problem.num_cities);
	int location_idx;

	/* do the cross breeding */
	tsp_cross_breed(fptr_crossover, fptr_selector, fptr_selector_init, from, to, start, end);

	/* mutate */
	for (i = start; i < end; i++)
	{
		/* mutate the copied genome */ 
		for (j = 0; j < num_mutations; j++)
		{
			location_idx = rand() % tsp_problem.num_cities;
			((int*)(to[i]->genome))[location_idx] = rand();
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_random_new_random_keys)
 * A random_keys code is [0..num_cities-1, 0..num_cities-1, ..., 1, 0]
 *-------------------------------------------------------------------------------------------*/
void tsp_random_new_random_keys(population_t **to, int start, int end)
{
	int i, j;

	for (i = start; i < end; i++)
	{
		for (j = 0; j < tsp_problem.num_cities; j++)
		{
			((int*)(to[i]->genome))[j] = rand();
		}
	}

}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_create_random_solution_random_keys)
 *-------------------------------------------------------------------------------------------*/
int *tsp_create_random_solution_random_keys()
{
	int *return_tour;
	int i;

	return_tour = (int *)malloc(sizeof(int) * tsp_problem.num_cities);

	for (i = 0; i < tsp_problem.num_cities; i++)
	{
		return_tour[i] = rand();
	}

	return return_tour;
}
