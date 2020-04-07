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

#include "genetic_algorithm_combinational_crossovers.h"

#include "analyze_genomes.h"

/*---------------------------------------------------------------------------------------------
 * (function: tsp_create_population)
 *-------------------------------------------------------------------------------------------*/
population_t **tsp_create_population_permutation()
{
	population_t **population;
	int i;

	population = (population_t **)malloc(sizeof(population_t*) * genomes.num_population);
	for (i = 0; i < genomes.num_population; i++)
	{
		population[i] = (population_t *)malloc(sizeof(population_t));
		population[i]->genome = (void*)tsp_create_random_solution_permutation();
	}

	return population;
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_free_population)
 *-------------------------------------------------------------------------------------------*/
void tsp_free_population()
{
	int i;

	for (i = 0; i < genomes.num_population; i++)
	{
		free(genomes.population[0][i]->genome);
		free(genomes.population[0][i]);
		free(genomes.population[1][i]->genome);
		free(genomes.population[1][i]);
	}
	free(genomes.population[0]);
	free(genomes.population[1]);
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_cost_function_and_order)
 *-------------------------------------------------------------------------------------------*/
void tsp_cost_function_and_order(
			double (*fptr_cost_function)(void *)
)
{
	int i, j;
	int index_of_lowest_cost;

	for (i = 0; i < genomes.num_population; i++)
	{
		genomes.costs[i] = (*fptr_cost_function)(genomes.population[global_index][i]->genome);
	}

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

	printf("Best solution costs: %f\n", genomes.costs[0]);
	global_best = genomes.costs[0];

#ifdef MEASURE_XOVER_RESULTS
	/* calculate average hamming distance between all pop and the best */
	float val = (*fptr_hamming_distance)(genomes.population[global_index], genomes.num_population, tsp_problem.num_cities);
	fprintf(ftest_out, "%s, Hamming Generation, %d, %f\n", (char*)global_args.config_file, breeding_cycles, val);

	/* bin the results depending on a set of bins */
	print_marks_bin_quality_of_results( genomes.costs, genomes.num_population);
#endif
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_cost_report_best)
 *-------------------------------------------------------------------------------------------*/
void tsp_cost_report_best(
		FILE *fout, 
		double (*fptr_cost_function)(void *)
		)
{
	float *costs;
	int i, j;
	int index_of_lowest_cost;

	costs = (float*)malloc(sizeof(float)*genomes.num_population);
	for (i = 0; i < genomes.num_population; i++)
	{
		costs[i] = (*fptr_cost_function)(genomes.population[global_index][i]->genome);
	}

	/* Sort the list of populations */
	for (i = 0; i < genomes.num_population - 1; i++)
	{
		index_of_lowest_cost = i;
		population_t *temp_pop;
		float temp_cost;	
		for (j = i + 1; j < genomes.num_population; j ++)
		{
			if (costs[index_of_lowest_cost] > costs[j])
				index_of_lowest_cost = j;
		}

		/* swap the top with new low */
		temp_pop = genomes.population[global_index][i];
		temp_cost = costs[i];

		genomes.population[global_index][i] = genomes.population[global_index][index_of_lowest_cost];
		costs[i] = costs[index_of_lowest_cost];

		genomes.population[global_index][index_of_lowest_cost] = temp_pop;
		costs[index_of_lowest_cost] = temp_cost;
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
 * (function: tsp_copy_old_genomes)
 *-------------------------------------------------------------------------------------------*/
void tsp_copy_old_genomes(population_t **from, population_t **to, int start, int end)
{
	int i;

	for (i = start; i < end; i++)
	{
		tsp_copy_solution((int*)from[i]->genome, (int*)to[i]->genome);
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_copy_solution)
 *-------------------------------------------------------------------------------------------*/
void tsp_copy_solution(int *from, int *to)
{
	int i;

	for (i = 0; i < tsp_problem.num_cities; i++)
	{
		to[i] = from[i];
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_cross_breed)
 *-------------------------------------------------------------------------------------------*/
void tsp_cross_breed(
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

		(*fptr_crossover)(genome_p1, genome_p2, genome_c1, genome_c2, tsp_problem.num_cities);

#ifdef MEASURE_XOVER_RESULTS
		switch(tsp_problem.problem_type)
		{
			case ADJACENCY_PERMUTATION:
				calculate_EBI_value_permutation(genome_p1, genome_p2, genome_c1, tsp_problem.num_cities);
				calculate_EBI_value_permutation(genome_p1, genome_p2, genome_c2, tsp_problem.num_cities);
				break;
			case ADJACENCY_LEHMER:
				calculate_EBI_value_lehmer(genome_p1, genome_p2, genome_c1, tsp_problem.num_cities);
				calculate_EBI_value_lehmer(genome_p1, genome_p2, genome_c2, tsp_problem.num_cities);
				break;
			case ADJACENCY_RANDOM_KEYS:
				calculate_EBI_value_random_keys(genome_p1, genome_p2, genome_c1, tsp_problem.num_cities);
				calculate_EBI_value_random_keys(genome_p1, genome_p2, genome_c2, tsp_problem.num_cities);
				break;
			default:
				printf("Not recognized tsp problem type!!!\n");
				return;
	}
#endif

		//tsp_check_genome(genome_c1, tsp_problem.num_cities);
		//tsp_check_genome(genome_c2, tsp_problem.num_cities);
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_mutate)
 *-------------------------------------------------------------------------------------------*/
void tsp_mutate(population_t **from, population_t **to, int start, int end, int from_best)
{
	int i, j;
	int who;
	int num_mutations = (int)floor(genomes.percent_of_genome_mutations * tsp_problem.num_cities);
	int swap_g1, swap_g2;
	int temp;

	for (i = start; i < end; i++)
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

/*---------------------------------------------------------------------------------------------
 * (function: tsp_mutate_no_copy)
 *-------------------------------------------------------------------------------------------*/
void tsp_mutate_no_copy(population_t **from, population_t **to, int start, int end, int from_best)
{
	int i, j;
	int who;
	int num_mutations = (int)floor(genomes.percent_of_genome_mutations * tsp_problem.num_cities);
	int swap_g1, swap_g2;
	int temp;

	for (i = start; i < end; i++)
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
/*---------------------------------------------------------------------------------------------
 * (function: tsp_breed_and_mutate)
 *-------------------------------------------------------------------------------------------*/
void tsp_breed_and_mutate(
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
	tsp_cross_breed(fptr_crossover, fptr_selector, fptr_selector_init, from, to, start, end);

	/* mutate */
	for (i = start; i < end; i++)
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

/*---------------------------------------------------------------------------------------------
 * (function: tsp_random_new)
 *-------------------------------------------------------------------------------------------*/
void tsp_random_new(population_t **to, int start, int end)
{
	int i, j;
	/* do num cities */
	int num_swaps;
	int temp;
	num_swaps = tsp_problem.num_cities * 2;

	for (i = start; i < end; i++)
	{
		for (j = 0; j < num_swaps; j++)
		{
			int swap1 = rand()%tsp_problem.num_cities;
			int swap2 = rand()%tsp_problem.num_cities;

			temp = ((int*)(to[i]->genome))[swap1];
			((int*)(to[i]->genome))[swap1] = ((int*)(to[i]->genome))[swap2];
			((int*)(to[i]->genome))[swap2] = temp;
		}
		//tsp_check_genome((int*)to[i]->genome, tsp_problem.num_cities);
	}

}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_exit_condition)
 *-------------------------------------------------------------------------------------------*/
short tsp_exit_condition()
{
	static int count = 0;
	static float recuring_best = 0;
	static int new_best = 0;

	breeding_cycles ++;
	printf("breed cycle: %d\n", breeding_cycles);
	fprintf(fresult_out, "%d", breeding_cycles);
	fflush(stdout);

	if (recuring_best == 0)
	{
		recuring_best = global_best;
	}
	else if (global_best < recuring_best)
	{
		recuring_best = global_best;
		new_best ++;
#ifdef CURRENT_BEST_RESULTS
		fprintf(ftest_out, "%s, generation, %d, count, %d, current_best, %f\n", (char*)global_args.config_file, breeding_cycles, new_best, global_best);
#endif
		count = 0;
	}
	else
	{
		count ++;
	}
	
#ifdef MEASURE_XOVER_RESULTS
	/* at this point we can cacluate average EBI as all crossovers have been totalled */
	fprintf(ftest_out, "%s, EBI_avg generation, %d, %f\n", (char*)global_args.config_file, breeding_cycles, ((float)EBI_total)/((float)EBI_count));
	/* reset for next cycle */
	EBI_total = 0;
	EBI_count = 0;
#endif

	if (configuration.exit_type == GENERATIONS)
	{
		if (breeding_cycles == 1000)
		{
			breeding_cycles = 0;
			return TRUE; // exit
		}
		else
		{
			return FALSE;
		}
	}
	else if (configuration.exit_type == TWO_HUNDRED_NO_BETTER)
	{
		if (count > 200)
		{
			fprintf(ftest_out, "%s, Exiting_generation, %d\n", (char*)global_args.config_file, breeding_cycles);
			count = 0;
			breeding_cycles = 0;
			return TRUE; // exit
		}
		else
		{
			return FALSE;
		}
	
	}
	else if (configuration.exit_type == FIVE_HUNDRED_NO_BETTER)
	{
		if (count > 500)
		{
			fprintf(ftest_out, "%s, Exiting_generation, %d\n", (char*)global_args.config_file, breeding_cycles);
			count = 0;
			breeding_cycles = 0;
			return TRUE; // exit
		}
		else
		{
			return FALSE;
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_create_random_solution)
 *-------------------------------------------------------------------------------------------*/
int *tsp_create_random_solution_permutation()
{
	int *return_tour;
	int i;
	int temp;
	/* do num cities * 2 random swaps */
	int num_swaps;
	num_swaps = tsp_problem.num_cities * 2;

	return_tour = (int *)malloc(sizeof(int) * tsp_problem.num_cities);

	for (i = 0; i < tsp_problem.num_cities; i++)
	{
		return_tour[i] = i;
	}

	for (i = 0; i < num_swaps; i++)
	{
		int swap1 = rand()%tsp_problem.num_cities;
		int swap2 = rand()%tsp_problem.num_cities;

		temp = return_tour[swap1];
		return_tour[swap1] = return_tour[swap2];
		return_tour[swap2] = temp;
	}

	return return_tour;
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_check_genome)
 *-------------------------------------------------------------------------------------------*/
void tsp_check_genome(int *test, int test_size)
{
	int *p1_lookup;
	int i;

	p1_lookup = (int*)malloc(sizeof(int) * test_size);

	for (i = 0; i < test_size; i++)
	{
		p1_lookup[i] = -1;
	}
	for (i = 0; i < test_size; i++)
	{
		if (p1_lookup[test[i]] != -1)
		{
			printf("Gene %d already present %d iteration\n", test[i], i);
			fflush(stdout);
			oassert(FALSE);
		}
		p1_lookup[test[i]] = i;
	}
	for (i = 0; i < test_size; i++)
	{
		if (p1_lookup[test[i]] == -1)
		{
			printf("Gene %d not present\n", i);
			fflush(stdout);
			oassert(FALSE);
		}
	}

	free(p1_lookup);
}

