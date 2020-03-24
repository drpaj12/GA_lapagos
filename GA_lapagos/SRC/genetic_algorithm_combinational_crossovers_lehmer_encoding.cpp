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

#include "genetic_algorithm_combinational_crossovers.h"
#include "genetic_algorithm_bit_crossovers.h"

/* Prototypes */

/* globals */

/*---------------------------------------------------------------------------------------------
 * (function: single_point_lehmer_xover)
 * Ucoluk, Gokturk - Genetic Algorithm Solution of the TSP Avoiding Special Crossover Mutation
 * - Did only one point
 *-------------------------------------------------------------------------------------------*/
void single_point_lehmer_xover(void *gp1, void *gp2, void *gc1, void *gc2, int genome_size)
{
	int i;

	int *genome_p1 = (int*)gp1;
	int *genome_p2 = (int*)gp2;
	int *genome_c1 = (int*)gc1;
	int *genome_c2 = (int*)gc2;

	/* cut point 1 needs to be in the first half */
	int cut_point1 = rand() % (genome_size);

	for (i = 0; i < genome_size; i++)
	{
		if (i < cut_point1)
		{
			genome_c1[i] = genome_p1[i];
			genome_c2[i] = genome_p2[i];
		}
		else
		{
			genome_c2[i] = genome_p1[i];
			genome_c1[i] = genome_p2[i];
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: two_point_lehmer_xover)
 * Ucoluk, Gokturk - Genetic Algorithm Solution of the TSP Avoiding Special Crossover Mutation
 * - Did only one point, so this is new
 *-------------------------------------------------------------------------------------------*/
void two_point_lehmer_xover(void *gp1, void *gp2, void *gc1, void *gc2, int genome_size)
{
	int i;

	int *genome_p1 = (int*)gp1;
	int *genome_p2 = (int*)gp2;
	int *genome_c1 = (int*)gc1;
	int *genome_c2 = (int*)gc2;

	int cut_point1 = rand() % (genome_size);
	int cut_point2 = rand() % (genome_size);
	int temp;

	if (cut_point2 > cut_point1)
	{
		temp = cut_point1;
		cut_point1 = cut_point2;
		cut_point2 = temp;
	}

	for (i = 0; i < genome_size; i++)
	{
		if (i < cut_point1 || i > cut_point2)
		{
			genome_c1[i] = genome_p1[i];
			genome_c2[i] = genome_p2[i];
		}
		else
		{
			genome_c2[i] = genome_p1[i];
			genome_c1[i] = genome_p2[i];
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: uniform_lehmer_xover)
 * Ucoluk, Gokturk - Genetic Algorithm Solution of the TSP Avoiding Special Crossover Mutation
 * - Did only one point, so this is new
 *-------------------------------------------------------------------------------------------*/
void uniform_lehmer_xover(void *gp1, void *gp2, void *gc1, void *gc2, int genome_size)
{
	int i;

	int *genome_p1 = (int*)gp1;
	int *genome_p2 = (int*)gp2;
	int *genome_c1 = (int*)gc1;
	int *genome_c2 = (int*)gc2;

	for (i = 0; i < genome_size; i++)
	{
		if (rand() % 2 == 0)
		{
			genome_c1[i] = genome_p1[i];
			genome_c2[i] = genome_p2[i];
		}
		else
		{
			genome_c2[i] = genome_p1[i];
			genome_c1[i] = genome_p2[i];
		}
	}
}

