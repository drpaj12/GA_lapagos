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
#include <time.h>
#include <sys/time.h>
#include "globals.h"
#include "types.h"
#include "utils.h"

#include "lehmer_encoding.h"
#include "random_key_encoding.h"

/*---------------------------------------------------------------------------------------------
 * (function: hamming_distance_best_versus_all_permutation)
 * Returns arithmetic average of hamming distance between all vs the best[0]
 * Note: assumes genome is an integer permuation - for lehmer or random keys you need to convert
 *-------------------------------------------------------------------------------------------*/
float hamming_distance_best_versus_all_permutation( population_t **pop, int population_size, int genome_size)
{
	float total;
	int i, j;
	int hamming_count;

	for (i = 1; i < population_size; i++)
	{
		hamming_count = 0;

		for (j = 0; j < genome_size; j++)
		{
			if (((int*)(pop[0]->genome))[j] != ((int*)(pop[i]->genome))[j])
			{
				hamming_count ++;
			}
		}

		total += hamming_count;
	}

	return total / (population_size-1);
}

/*---------------------------------------------------------------------------------------------
 * (function: hamming_distance_best_versus_all_lehmer)
 *-------------------------------------------------------------------------------------------*/
float hamming_distance_best_versus_all_lehmer( population_t **pop, int population_size, int genome_size)
{ 
	population_t **temp_pop;
	float total;
	int i, j;
	int *decoded;

	/* Alocate and copy */
	temp_pop = (population_t **)malloc(sizeof(population_t*) * population_size);
	for (i = 0; i < population_size; i++)
	{
		temp_pop[i] = (population_t *)malloc(sizeof(population_t));
		temp_pop[i]->genome = (void*)malloc(sizeof(int) * genome_size);

		/* Lehmer decode and store in array */
		decoded = lehmer_decode_faster((int*)pop[i]->genome, genome_size);
		for (j = 0; j < genome_size; j++)
		{
			((int*)temp_pop[i]->genome)[j] = decoded[j];
			free(decoded);
		}
	}

	/* calculate after decoding */
	total = hamming_distance_best_versus_all_permutation(temp_pop, population_size, genome_size);

	/* free everything */
	for (i = 0; i < population_size; i++)
	{
		free(temp_pop[i]->genome);
		free(temp_pop[i]);
	}
	free(temp_pop);

	return total;
}

/*---------------------------------------------------------------------------------------------
 * (function: hamming_distance_best_versus_all_random_key)
 *-------------------------------------------------------------------------------------------*/
float hamming_distance_best_versus_all_random_key( population_t **pop, int population_size, int genome_size)
{ 
	population_t **temp_pop;
	float total;
	int i, j;
	int *decoded;

	/* Alocate and copy */
	temp_pop = (population_t **)malloc(sizeof(population_t*) * population_size);
	for (i = 0; i < population_size; i++)
	{
		temp_pop[i] = (population_t *)malloc(sizeof(population_t));
		temp_pop[i]->genome = (void*)malloc(sizeof(int) * genome_size);
		/* Random_key decode and store in array */
		decoded = random_key_decode((int*)pop[i]->genome, genome_size);
		for (j = 0; j < genome_size; j++)
		{
			((int*)temp_pop[i]->genome)[j] = decoded[j];
			free(decoded);
		}
	}

	/* calculate after decoding */
	total = hamming_distance_best_versus_all_permutation(temp_pop, population_size, genome_size);

	/* free everything */
	for (i = 0; i < population_size; i++)
	{
		free(temp_pop[i]->genome);
		free(temp_pop[i]);
	}
	free(temp_pop);

	return total;
}
