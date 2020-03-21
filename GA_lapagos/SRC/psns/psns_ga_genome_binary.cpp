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

#include "psns_globals.h"
#include "psns_types.h"
#include "psns_dat.h"

#include "psns_ga_genome_binary.h"

#include "genetic_algorithm_combinational_crossovers.h"

/*---------------------------------------------------------------------------------------------
 * (function: psns_create_population)
 *-------------------------------------------------------------------------------------------*/
population_t **psns_create_population_bits()
{
	population_t **population;
	int i;

	population = (population_t **)malloc(sizeof(population_t*) * genomes.num_population);
	for (i = 0; i < genomes.num_population; i++)
	{
		population[i] = (population_t *)malloc(sizeof(population_t));
		population[i]->genome = (void*)psns_create_random_solution_bits();
	}

	return population;
}

/*---------------------------------------------------------------------------------------------
 * (function: psns_free_population)
 *-------------------------------------------------------------------------------------------*/
void psns_free_population_bits()
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
 * (function: psns_copy_old_genomes)
 *-------------------------------------------------------------------------------------------*/
void psns_copy_old_genomes_bits(population_t **from, population_t **to, int start, int end)
{
	int i;

	for (i = start; i < end; i++)
	{
		psns_copy_solution_bits((bstr)from[i]->genome, (bstr)to[i]->genome);
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: psns_copy_solution)
 *-------------------------------------------------------------------------------------------*/
void psns_copy_solution_bits(bstr from, bstr to)
{
	int i;

	bitstr_copy(from, to);
}

/*---------------------------------------------------------------------------------------------
 * (function: psns_cross_breed)
 *-------------------------------------------------------------------------------------------*/
void psns_cross_breed_bits(
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

		(*fptr_crossover)(genome_p1, genome_p2, genome_c1, genome_c2, psns_genome_bits.num_bits);
	}
}
/*---------------------------------------------------------------------------------------------
 * (function: psns_mutate)
 *-------------------------------------------------------------------------------------------*/
void psns_mutate_bits(population_t **from, population_t **to, int start, int end, int from_best)
{
	int i, j;
	int who;
	int num_mutations = (int)floor(genomes.percent_of_genome_mutations * psns_genome_bits.num_bits);
	int idx_to_mutate;

	for (i = start; i < end; i++)
	{
		/* find out who is the parent, copy over to "to" */
		who = rand() % from_best;
		psns_copy_solution_bits((bstr)from[who]->genome, (bstr)to[i]->genome);

		/* mutate the copied genome */ 
		for (j = 0; j < num_mutations; j++)
		{
			idx_to_mutate = rand() % psns_genome_bits.num_bits;
			bitstr_flip((bstr)to[i]->genome, idx_to_mutate);
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: psns_breed_and_mutate)
 *-------------------------------------------------------------------------------------------*/
void psns_breed_and_mutate_bits(
							void (*fptr_crossover)(void *, void *, void *, void *, int),
							int (*fptr_selector)(),
							void (*fptr_selector_init)(int),
							population_t **from, 
							population_t **to, 
							int start, 
							int end)
{
	int i, j;
	int num_mutations = (int)floor(genomes.percent_of_genome_mutations * psns_genome_bits.num_bits);
	int idx_to_mutate;

	/* do the cross breeding */
	psns_cross_breed_bits(fptr_crossover, fptr_selector, fptr_selector_init, from, to, start, end);

	/* mutate */
	for (i = start; i < end; i++)
	{
		/* mutate the copied genome */ 
		for (j = 0; j < num_mutations; j++)
		{
			idx_to_mutate = rand() % psns_genome_bits.num_bits;
			bitstr_flip((bstr)to[i]->genome, idx_to_mutate);
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: psns_create_random_solution)
 *-------------------------------------------------------------------------------------------*/
void psns_random_new_bits(population_t **to, int start, int end)
{
	int i, j, k;
	int bit_index;

	for (i = start; i < end; i++)
	{
		bit_index = 0;

		for (j = 0; j < psns_problem.num_base_stations; j++)
		{
			for (k = 0; k < psns_genome_bits.num_bits_x; k++)
			{
				if (rand()%2 == 0)
					bitstr_clear((bstr)to[i]->genome, bit_index + k);
				else
					bitstr_set((bstr)to[i]->genome, bit_index + k);
			}
			for (k = 0; k < psns_genome_bits.num_bits_y; k++)
			{
				if (rand()%2 == 0)
					bitstr_clear((bstr)to[i]->genome, bit_index + k);
				else
					bitstr_set((bstr)to[i]->genome, bit_index + k);
			}
	
			bit_index += psns_genome_bits.num_bits_xy;
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: psns_create_random_solution)
 *-------------------------------------------------------------------------------------------*/
void *psns_create_random_solution_bits()
{
	bstr return_genome;
	int i, j;
	int bit_index = 0;

	return_genome = bitstr_new(psns_genome_bits.num_bits) ;

	for (i = 0; i < psns_problem.num_base_stations; i++)
	{
		for (j = 0; j < psns_genome_bits.num_bits_x; j++)
		{
			if (rand()%2 == 0)
				bitstr_clear(return_genome, bit_index + j);
			else
				bitstr_set(return_genome, bit_index + j);
		}
		bit_index += psns_genome_bits.num_bits_x;
		for (j = 0; j < psns_genome_bits.num_bits_y; j++)
		{
			if (rand()%2 == 0)
				bitstr_clear(return_genome, bit_index + j);
			else
				bitstr_set(return_genome, bit_index + j);
		}

		bit_index += psns_genome_bits.num_bits_y;
	}

	return return_genome;
}
