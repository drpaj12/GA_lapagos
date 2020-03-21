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

#include "genetic_algorithm_bit_crossovers.h"

/* Prototypes */

/* globals */

/*---------------------------------------------------------------------------------------------
 * (function: xover_single_point)
 * M. Mitchell. 1999. An Introduction to Genetic Algorithms. The MIT Press, Cambridge, MA.
 *-------------------------------------------------------------------------------------------*/
void xover_single_point(void *genome_p1, void *genome_p2, void *genome_c1, void *genome_c2, int genome_size)
{
	/* cut point 1 needs to be in the first half */
	int cut_point1 = rand() % (genome_size);

	bitstr_copy_substr((bstr)genome_p1, (bstr)genome_c1, 0, cut_point1);
	bitstr_copy_substr((bstr)genome_p2, (bstr)genome_c1, cut_point1, genome_size);
	bitstr_copy_substr((bstr)genome_p2, (bstr)genome_c2, 0, cut_point1);
	bitstr_copy_substr((bstr)genome_p1, (bstr)genome_c2, cut_point1, genome_size);

#if 0
	printf("cut_point1 = %d\n", cut_point1);
	bitstr_print((bstr)genome_p1);
	bitstr_print((bstr)genome_p2);
	bitstr_print((bstr)genome_c1);
	bitstr_print((bstr)genome_c2);
#endif
}

/*---------------------------------------------------------------------------------------------
 * (function: xover_two_point)
 * Larry J. Eshelman, Richard A. Caruana, and J. David Schaffer. 1989. Biases in the crossover landscape. In
 * Proceedings of the 3rd International Conference on Genetic Algorithms. Morgan Kaufman, 10–19.
 *-------------------------------------------------------------------------------------------*/
void xover_two_point(void *genome_p1, void *genome_p2, void *genome_c1, void *genome_c2, int genome_size)
{
	/* cut point 1 needs to be in the first half */
	int cut_point1 = rand() % (genome_size);
	int cut_point2 = rand() % (genome_size);
	int big, little;

	if (cut_point1 == cut_point2)
	{
		little = cut_point1;
		big = cut_point2 + 1;
	}
	else if (cut_point1 > cut_point2)
	{
		little = cut_point1;
		big = cut_point2;
	}
	else
	{
		little = cut_point2;
		big = cut_point1;
	}

	if (little == 0 && big == genome_size)
	{
		bitstr_copy_substr((bstr)genome_p1, (bstr)genome_c1, 0, genome_size);
		bitstr_copy_substr((bstr)genome_p2, (bstr)genome_c2, 0, genome_size);
	}
	else if (little == 0)
	{
		bitstr_copy_substr((bstr)genome_p1, (bstr)genome_c1, 0, big);
		bitstr_copy_substr((bstr)genome_p2, (bstr)genome_c2, 0, big);
		bitstr_copy_substr((bstr)genome_p1, (bstr)genome_c2, big, genome_size);
		bitstr_copy_substr((bstr)genome_p2, (bstr)genome_c1, big, genome_size);
	}
	else if (big == genome_size)
	{
		bitstr_copy_substr((bstr)genome_p1, (bstr)genome_c1, 0, little);
		bitstr_copy_substr((bstr)genome_p2, (bstr)genome_c2, 0, little);
		bitstr_copy_substr((bstr)genome_p1, (bstr)genome_c2, little, genome_size);
		bitstr_copy_substr((bstr)genome_p2, (bstr)genome_c1, little, genome_size);
	}
	else
	{
		bitstr_copy_substr((bstr)genome_p1, (bstr)genome_c1, 0, little);
		bitstr_copy_substr((bstr)genome_p2, (bstr)genome_c2, 0, little);
		bitstr_copy_substr((bstr)genome_p1, (bstr)genome_c2, little, big);
		bitstr_copy_substr((bstr)genome_p2, (bstr)genome_c1, little, big);
		bitstr_copy_substr((bstr)genome_p1, (bstr)genome_c1, big, genome_size);
		bitstr_copy_substr((bstr)genome_p2, (bstr)genome_c2, big, genome_size);
	}

#if 0
	printf("cut_point1 = %d; cut_point2 = %d\n", cut_point1, cut_point2);
	bitstr_print((bstr)genome_p1);
	bitstr_print((bstr)genome_p2);
	bitstr_print((bstr)genome_c1);
	bitstr_print((bstr)genome_c2);
#endif
}

/*---------------------------------------------------------------------------------------------
 * (function: xover_uniform)
 *-------------------------------------------------------------------------------------------*/
void xover_uniform(void *genome_p1, void *genome_p2, void *genome_c1, void *genome_c2, int genome_size)
{
	int i; 

	for (i = 0; i < genome_size; i++)
	{
		if (rand() % 2 == 0)
		{
			bitstr_bitcopy((bstr)genome_p1, (bstr)genome_c1, i);
			bitstr_bitcopy((bstr)genome_p2, (bstr)genome_c2, i);
		}
		else
		{
			bitstr_bitcopy((bstr)genome_p1, (bstr)genome_c2, i);
			bitstr_bitcopy((bstr)genome_p2, (bstr)genome_c1, i);
		}
	}

#if 0
	bitstr_print((bstr)genome_p1);
	bitstr_print((bstr)genome_p2);
	bitstr_print((bstr)genome_c1);
	bitstr_print((bstr)genome_c2);
#endif
}
