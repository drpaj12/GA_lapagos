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

#include "thread_utils.h"
#include "tsp_globals.h"
#include "tsp_types.h"

#include "tsp_ga_genome_combinatorial_tupple.h"
#include "tsp_ga_genome_combinatorial_tupple_threaded.h"

#include "genetic_algorithm_combinational_crossovers.h"

/*---------------------------------------------------------------------------------------------
 * (function: tsp_cost_function_and_order)
 *-------------------------------------------------------------------------------------------*/
void tsp_cost_function_and_order_threaded(
		thread_t *thread_struct,
		double (*fptr_cost_function)(void *)
		)
{
	int i, j;
	int index_of_lowest_cost;

	for (i = 0; i < genomes.num_population; i++)
	{
		if (i % configuration.num_threads == thread_struct->id)
			genomes.costs[i] = (*fptr_cost_function)(genomes.population[global_index][i]->genome);
	}

	/* wait for all to calculate */
	barrier(thread_struct);

	if (thread_struct->id == 0)
	{
		/* Sort the list of populations */
		for (i = 0; i < genomes.num_population - 1; i++)
		{
			index_of_lowest_cost = i;
			population_t *temp_pop;
			float temp_cost;	
			for (j = i + 1; j < genomes.num_population; j ++)
			{
				if (genomes.costs[index_of_lowest_cost] > genomes.costs[j])
					index_of_lowest_cost = j;
			}
	
			/* swap the top with new low */
			temp_pop = genomes.population[global_index][i];
			temp_cost = genomes.costs[i];
	
			genomes.population[global_index][i] = genomes.population[global_index][index_of_lowest_cost];
			genomes.costs[i] = genomes.costs[index_of_lowest_cost];
	
			genomes.population[global_index][index_of_lowest_cost] = temp_pop;
			genomes.costs[index_of_lowest_cost] = temp_cost;
		}

		printf("Thread 0 says: Best solution costs: %f\n", genomes.costs[0]);
		global_best = genomes.costs[0];
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_cross_breed)
 *-------------------------------------------------------------------------------------------*/
void tsp_cross_breed_threaded(
							thread_t *thread_struct,
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
	int temp;
	int *genome_p1;
	int *genome_p2;
	int *genome_c1;
	int *genome_c2;

	if (start == end)
		return;

	pthread_mutex_lock(thread_struct->lock);

	/* initialize the selector */
	(*fptr_selector_init)(end-start);

	pthread_mutex_unlock(thread_struct->lock);

	int temp_end = end-(end - start)%2; // make sure even number
	for (i = start; i < temp_end; i += 2)
	{
		if (i/2 % configuration.num_threads == thread_struct->id)
		{
			/* pick up 2 parents */
			pthread_mutex_lock(thread_struct->lock);
			parent1 = (*fptr_selector)();
			pthread_mutex_unlock(thread_struct->lock);
			pthread_mutex_lock(thread_struct->lock);
			parent2 = (*fptr_selector)();
			pthread_mutex_unlock(thread_struct->lock);
	
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
	
			(*fptr_crossover)(genome_p1, genome_p2, genome_c1, genome_c2, tsp_problem.num_cities);
	
			tsp_check_genome(genome_c1, tsp_problem.num_cities);
			tsp_check_genome(genome_c2, tsp_problem.num_cities);
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_mutate)
 *-------------------------------------------------------------------------------------------*/
void tsp_mutate_threaded(thread_t *thread_struct, population_t **from, population_t **to, int start, int end, int from_best)
{
	int i, j;
	int who;
	int num_mutations = (int)floor(genomes.percent_of_genome_mutations * tsp_problem.num_cities);
	int swap_g1, swap_g2;
	int temp;

	for (i = start; i < end; i++)
	{
		if (i % configuration.num_threads == thread_struct->id)
		{
			/* find out who is the parent, copy over to "to" */
			who = rand() % from_best;
			tsp_copy_solution((int*)from[who]->genome, (int*)to[i]->genome);
	
			/* mutate the copied genome */ 
			for (j = 0; j < num_mutations; j++)
			{
				swap_g1 = rand() % tsp_problem.num_cities;
				swap_g2 = rand() % tsp_problem.num_cities;
				temp = ((int*)(to[i]->genome))[swap_g1];
				((int*)(to[i]->genome))[swap_g1] = ((int*)(to[i]->genome))[swap_g2];
				((int*)(to[i]->genome))[swap_g2] = temp;
			}
		}
	}
}
/*---------------------------------------------------------------------------------------------
 * (function: tsp_mutate)
 *-------------------------------------------------------------------------------------------*/
void tsp_mutate_no_copy_threaded(thread_t *thread_struct, population_t **from, population_t **to, int start, int end, int from_best)
{
	int i, j;
	int num_mutations = (int)floor(genomes.percent_of_genome_mutations * tsp_problem.num_cities);
	int swap_g1, swap_g2;
	int temp;

	for (i = start; i < end; i++)
	{
		if (i % configuration.num_threads == thread_struct->id)
		{
			/* mutate the copied genome */ 
			for (j = 0; j < num_mutations; j++)
			{
				swap_g1 = rand() % tsp_problem.num_cities;
				swap_g2 = rand() % tsp_problem.num_cities;
				temp = ((int*)(to[i]->genome))[swap_g1];
				((int*)(to[i]->genome))[swap_g1] = ((int*)(to[i]->genome))[swap_g2];
				((int*)(to[i]->genome))[swap_g2] = temp;
			}
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_breed_and_mutate)
 *-------------------------------------------------------------------------------------------*/
void tsp_breed_and_mutate_threaded(
							thread_t *thread_struct,
							void (*fptr_crossover)(void *, void *, void *, void *, int),
							int (*fptr_selector)(),
							void (*fptr_selector_init)(int),
							population_t **from, 
							population_t **to, 
							int start, 
							int end)
{
	int i, j;
	int num_mutations = (int)floor(genomes.percent_of_genome_mutations * tsp_problem.num_cities);
	int swap_g1, swap_g2;
	int temp;

	/* do the cross breeding */
	tsp_cross_breed_threaded(thread_struct, fptr_crossover, fptr_selector, fptr_selector_init, from, to, start, end);

	int temp_end = end-(end - start)%2; // make sure even number
	for (i = start; i < temp_end; i += 2)
	{
		if (i/2 % configuration.num_threads == thread_struct->id)
		{
			/* mutate the copied genome */ 
			for (j = 0; j < num_mutations; j++)
			{
				swap_g1 = rand() % tsp_problem.num_cities;
				swap_g2 = rand() % tsp_problem.num_cities;
				temp = ((int*)(to[i]->genome))[swap_g1];
				((int*)(to[i]->genome))[swap_g1] = ((int*)(to[i]->genome))[swap_g2];
				((int*)(to[i]->genome))[swap_g2] = temp;
			}
			/* mutate second copied genome */ 
			for (j = 0; j < num_mutations; j++)
			{
				swap_g1 = rand() % tsp_problem.num_cities;
				swap_g2 = rand() % tsp_problem.num_cities;
				temp = ((int*)(to[i+1]->genome))[swap_g1];
				((int*)(to[i+1]->genome))[swap_g1] = ((int*)(to[i+1]->genome))[swap_g2];
				((int*)(to[i+1]->genome))[swap_g2] = temp;
			}
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_random_new)
 *-------------------------------------------------------------------------------------------*/
void tsp_random_new_threaded(thread_t *thread_struct, population_t **to, int start, int end)
{
	int i, j;
	/* do num cities */
	int num_swaps;
	int temp;
	num_swaps = tsp_problem.num_cities * 2;

	for (i = start; i < end; i++)
	{
		if (i % configuration.num_threads == thread_struct->id)
		{
			for (j = 0; j < num_swaps; j++)
			{
				int swap1 = rand()%tsp_problem.num_cities;
				int swap2 = rand()%tsp_problem.num_cities;
		
				temp = ((int*)(to[i]->genome))[swap1];
				((int*)(to[i]->genome))[swap1] = ((int*)(to[i]->genome))[swap2];
				((int*)(to[i]->genome))[swap2] = temp;
			}
			tsp_check_genome((int*)to[i]->genome, tsp_problem.num_cities);
		}
	}

}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_copy_old_genomes)
 *-------------------------------------------------------------------------------------------*/
void tsp_copy_old_genomes_threaded(thread_t *thread_struct, population_t **from, population_t **to, int start, int end)
{
	int i;

	for (i = start; i < end; i++)
	{
		if (i % configuration.num_threads == thread_struct->id)
		{
			tsp_copy_solution((int*)from[i]->genome, (int*)to[i]->genome);
		}
	}
}


