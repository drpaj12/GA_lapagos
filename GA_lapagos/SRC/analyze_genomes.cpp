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
 * (function: hamming_distance_best_versus_all_random_keys)
 *-------------------------------------------------------------------------------------------*/
float hamming_distance_best_versus_all_random_keys( population_t **pop, int population_size, int genome_size)
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

/*---------------------------------------------------------------------------------------------
 * (function: print_marks_bin_quality_of_results)
 * Prints out the quality distribution into 5 bins based on BEST and WORST value 5, 4, 3, 2, 1
 * Basically does (|BEST - WORST| / 5) to find gap.  5 is (BEST to BEST +/ - gap), 4 is (High +/- gap to High +/- 2*gap), etc.
 * Then printout
 * Note TSP is a decreasing optimization so BEST is smaller
 *-------------------------------------------------------------------------------------------*/
void print_marks_bin_quality_of_results( double *costs, int population_size)
{
	int num_bins = 5;
	int *bins;
	double best = costs[0];
	double worst = costs[population_size-1];
	double gap = (low - high) / num_bins;
	int i, j;

	bins = (int*)malloc(sizeof(int) * num_bins);
	for (j = 0; j < num_bins; j++)
		bins[j] = 0;

	/* count which bin cost function is in */
	for (i = 0; i < population_size; i++)
	{
		for (j = 0; j < num_bins; j++)
		{
			if (costs[i] <= best + (gap * (j+1))) 
			{
				int bin_num_index = num_bins - j - 1;
				bins[bin_num_index] ++;
				break;
			}

		}
	}

	/* print out the details of the distribution */
	fprintf(ftest_out, "%s, Marks Generation (Big to Small), %d, ", (char*)global_args.config_file, breeding_cycles);
	for (j = 0; j < num_bins; j++)
	{
		fprintf(ftest_out, "%d", bins[num_bins - j - 1]);
	}
	fprintf(ftest_out, "\n");

	free(bins);
}

/*---------------------------------------------------------------------------------------------
 * (function: calculate_EBI_value_permutation)
 * Returns arithmetic average of hamming distance between all vs the best[0]
 * Note: assumes genome is an integer permuation - for lehmer or random keys you need to convert
 *-------------------------------------------------------------------------------------------*/
void calculate_EBI_value_permutation( population_t **pop, int genome_size)
{

