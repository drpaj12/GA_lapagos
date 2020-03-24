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
 * (function: xover_pmx)
 * Goldberg 1985 "Alleles, loci, and the Travelling saleman problem"
 *-------------------------------------------------------------------------------------------*/
void xover_pmx(void *gp1, void *gp2, void *gc1, void *gc2, int genome_size)
{
	int i, j;
	short *list_of_idx_to_gene_spots_to_keep;
	int *p1_lookup;
	int *p2_lookup;

	int *genome_p1 = (int*)gp1;
	int *genome_p2 = (int*)gp2;
	int *genome_c1 = (int*)gc1;
	int *genome_c2 = (int*)gc2;

	/* cut point 1 needs to be in the first half */
	int cut_point1 = rand() % (genome_size/2);
	int cut_point2 = cut_point1 + (genome_size/2);

	list_of_idx_to_gene_spots_to_keep = (short*)malloc(sizeof(short) * genome_size);
	p1_lookup = (int*)malloc(sizeof(int) * genome_size);
	p2_lookup = (int*)malloc(sizeof(int) * genome_size);
	for (i = 0; i < genome_size; i++)
	{
		if (i > cut_point1 || i < cut_point2)
		{
			list_of_idx_to_gene_spots_to_keep[i] = TRUE;
			genome_c1[i] = genome_p1[i];
			genome_c2[i] = genome_p2[i];
		}
		else
			list_of_idx_to_gene_spots_to_keep[i] = FALSE;
	}

	/* create the reverse lookup...as in if i want to know where city 3 is in genome p1 I can write p1_lookup[3] */
	for (j = 0; j < genome_size; j++)
	{
		p1_lookup[genome_p1[j]] = j;
		p2_lookup[genome_p2[j]] = j;
	}

	/* now copy over the parents details with a remapping for overlaps */
	for (j = 0; j < genome_size; j++)
	{
		/* copy c1 from p2 */
		if (list_of_idx_to_gene_spots_to_keep[j] == FALSE)
		{
			int genome_id_for_here = genome_p2[j];
			/* If this is not true then get from parent2 */
			while (list_of_idx_to_gene_spots_to_keep[p1_lookup[genome_id_for_here]] == TRUE)
			{
				genome_id_for_here = genome_p2[p1_lookup[genome_id_for_here]];
			}
			genome_c1[j] = genome_id_for_here;
		}

		/* copy c2 from p1 */
		if (list_of_idx_to_gene_spots_to_keep[j] == FALSE)
		{
			int genome_id_for_here = genome_p1[j];
			/* If this is not true then get from parent2 */
			while (list_of_idx_to_gene_spots_to_keep[p2_lookup[genome_id_for_here]] == TRUE)
			{
				genome_id_for_here = genome_p1[p2_lookup[genome_id_for_here]];
			}
			genome_c2[j] = genome_id_for_here;
		}
	}

	/* cleanup */
	free(list_of_idx_to_gene_spots_to_keep);
	free(p1_lookup);
	free(p2_lookup);
}

/*---------------------------------------------------------------------------------------------
 * (function: xover_position)
 * Syswerda 1990 "Schedule Optimization Using Genetic Algorithms"
 * PMX like but random set is chosen and then mapping done after
 *-------------------------------------------------------------------------------------------*/
void xover_position(void *gp1, void *gp2, void *gc1, void *gc2, int genome_size)
{
	int i, j;
	short *list_of_idx_to_gene_spots_to_keep;
	int temp;
	int *p1_lookup;
	int *p2_lookup;
	int swap_idx;

	int *genome_p1 = (int*)gp1;
	int *genome_p2 = (int*)gp2;
	int *genome_c1 = (int*)gc1;
	int *genome_c2 = (int*)gc2;

	list_of_idx_to_gene_spots_to_keep = (short*)malloc(sizeof(short) * genome_size);
	p1_lookup = (int*)malloc(sizeof(int) * genome_size);
	p2_lookup = (int*)malloc(sizeof(int) * genome_size);
	for (i = 0; i < genome_size; i++)
	{
		if (i < genome_size/2)
			list_of_idx_to_gene_spots_to_keep[i] = TRUE;
		else
			list_of_idx_to_gene_spots_to_keep[i] = FALSE;
	}

	/* create the reverse lookup...as in if i want to know where city 3 is in genome p1 I can write p1_lookup[3] */
	for (j = 0; j < genome_size; j++)
	{
		p1_lookup[genome_p1[j]] = j;
		p2_lookup[genome_p2[j]] = j;
	}

	/* mix up the ones to keep */
	for (j = 0; j < genome_size/2; j++)
	{
		swap_idx = rand() % genome_size;
		temp = list_of_idx_to_gene_spots_to_keep[swap_idx];
		list_of_idx_to_gene_spots_to_keep[swap_idx] = list_of_idx_to_gene_spots_to_keep[j];
		list_of_idx_to_gene_spots_to_keep[j] = temp;
	}

	/* now copy over the parents details with a remapping for overlaps */
	for (j = 0; j < genome_size; j++)
	{
		/* copy c1 from p2 */
		if (list_of_idx_to_gene_spots_to_keep[j] == FALSE)
		{
			int genome_id_for_here = genome_p2[j];
			/* If this is not true then get from parent2 */
			while (list_of_idx_to_gene_spots_to_keep[p1_lookup[genome_id_for_here]] == TRUE)
			{
				genome_id_for_here = genome_p2[p1_lookup[genome_id_for_here]];
			}
			genome_c1[j] = genome_id_for_here;
		}
		else
		{
			genome_c1[j] = genome_p1[j];
		}	

		/* copy c2 from p1 */
		if (list_of_idx_to_gene_spots_to_keep[j] == FALSE)
		{
			int genome_id_for_here = genome_p1[j];
			/* If this is not true then get from parent2 */
			while (list_of_idx_to_gene_spots_to_keep[p2_lookup[genome_id_for_here]] == TRUE)
			{
				genome_id_for_here = genome_p1[p2_lookup[genome_id_for_here]];
			}
			genome_c2[j] = genome_id_for_here;
		}
		else
		{
			genome_c2[j] = genome_p2[j];
		}	
	}

	/* cleanup */
	free(list_of_idx_to_gene_spots_to_keep);
	free(p1_lookup);
	free(p2_lookup);
}

/*---------------------------------------------------------------------------------------------
 * (function: xover_cx)
 * Oliver 1987 "A Study of Permutation Crossover on the Travelling Salesman Problem"
 * Cycle crossover function (also known as CX) on the int permutations
 * proposed by Oliver [15] builds offspring in such a way that each city (and its position) 
 * comes from one of the parents
 * Note: One cycle is made, and then all the other data is copied from the opposite parent.	 
 *-------------------------------------------------------------------------------------------*/
void xover_cx(void *gp1, void *gp2, void *gc1, void *gc2, int genome_size)
{
	int i;
	short *list_of_idx_to_gene_spots_to_keep;
	int temp;
	int *p2_lookup;
	int swap_idx;

	int *genome_p1 = (int*)gp1;
	int *genome_p2 = (int*)gp2;
	int *genome_c1 = (int*)gc1;
	int *genome_c2 = (int*)gc2;

	int tour_index;
	int start_tour_index;
	int tour2_idx;

	list_of_idx_to_gene_spots_to_keep = (short*)calloc(genome_size, sizeof(short)); // sets to FALSE
	p2_lookup = (int*)malloc(sizeof(int) * genome_size);

	/* create the reverse lookup...as in if i want to know where city 3 is in genome p1 I can write p1_lookup[3] */
	for (i = 0; i < genome_size; i++)
	{
		p2_lookup[genome_p2[i]] = i;
	}

	/* pick a random spot to start tour */
	tour_index = rand() % genome_size;

	start_tour_index = tour_index;
	/* mark spot done */
	list_of_idx_to_gene_spots_to_keep[tour_index] = TRUE;
	/* copy location into the child */
	genome_c1[tour_index] = genome_p1[tour_index];
	/* find the new spot in parent2 */
	tour2_idx = p2_lookup[genome_p1[tour_index]];
	/* copy in parent 2 since eventually this spot will be on tour */
	genome_c2[tour2_idx] = genome_p2[tour2_idx];
	/* update next tour spot */
	tour_index = tour2_idx;

	while (tour_index != start_tour_index)
	{
		/* mark spot done */
		list_of_idx_to_gene_spots_to_keep[tour_index] = TRUE;
		/* copy location into the child */
		genome_c1[tour_index] = genome_p1[tour_index];
		/* find the new spot in parent2 */
		tour2_idx = p2_lookup[genome_p1[tour_index]];
		/* copy in parent 2 since eventually this spot will be on tour */
		genome_c2[tour2_idx] = genome_p2[tour2_idx];
		/* update next tour spot */
		tour_index = tour2_idx;
	}

	/* now copy over the parents details with a remapping for overlaps */
	for (i = 0; i < genome_size; i++)
	{
		/* copy c1 from p2 and c2 from p1 */
		if (list_of_idx_to_gene_spots_to_keep[i] == FALSE)
		{
			/* If this is not true then get from parent2 */
			genome_c1[i] = genome_p2[i];
			
			/* If this is not true then get from parent1 */
			genome_c2[i] = genome_p1[i];
		}
	}

	/* cleanup */
	free(list_of_idx_to_gene_spots_to_keep);
	free(p2_lookup);
}

/*---------------------------------------------------------------------------------------------
 * (function: xover_cx2)
 * Extension of cx to make all cycles
 * Hussain 2017 Genetic Algorithm for Traveling Salesman Problem with Modified Cycle Crossover Operator
 *-------------------------------------------------------------------------------------------*/
void xover_cx2(void *gp1, void *gp2, void *gc1, void *gc2, int genome_size)
{
	int i;
	int *p2_lookup;
	int num_rand_start_points;
	int *list_of_rand_start_points;
	int *rand_lookup;
	int rand_start_point;

	int *genome_p1 = (int*)gp1;
	int *genome_p2 = (int*)gp2;
	int *genome_c1 = (int*)gc1;
	int *genome_c2 = (int*)gc2;

	int tour_index;
	int start_tour_index;
	int tour2_idx;

	short swap = 0;

	p2_lookup = (int*)malloc(sizeof(int) * genome_size);
	list_of_rand_start_points = (int*)malloc(sizeof(int) * genome_size);
	rand_lookup = (int*)malloc(sizeof(int) * genome_size);

	/* create the reverse lookup...as in if i want to know where city 3 is in genome p1 I can write p1_lookup[3] */
	for (i = 0; i < genome_size; i++)
	{
		p2_lookup[genome_p2[i]] = i;
		list_of_rand_start_points[i] = i;
		rand_lookup[i] = i;
	}

	/* set the list of rand points */
	num_rand_start_points = genome_size;

	while (num_rand_start_points > 0)
	{
		rand_start_point = rand() % num_rand_start_points;
		/* pick a random spot to start tour */
		tour_index = list_of_rand_start_points[rand_start_point];

		/* remove possible tour start - swap the bottom element of the tour list */
		list_of_rand_start_points[rand_lookup[tour_index]] = list_of_rand_start_points[num_rand_start_points-1];
		rand_lookup[list_of_rand_start_points[num_rand_start_points-1]] = rand_lookup[tour_index];
		num_rand_start_points --;
	
		start_tour_index = tour_index;

		/* copy location into the child */
		if (swap == 0)
			genome_c1[tour_index] = genome_p1[tour_index];
		else
			genome_c1[tour_index] = genome_p2[tour_index];

		/* find the new spot in parent2 */
		tour2_idx = p2_lookup[genome_p1[tour_index]];
		/* copy in parent 2 since eventually this spot will be on tour */
		if (swap == 0)
			genome_c2[tour2_idx] = genome_p2[tour2_idx];
		else
			genome_c2[tour2_idx] = genome_p1[tour2_idx];
		/* update next tour spot */
		tour_index = tour2_idx;
	
		while (tour_index != start_tour_index)
		{
			/* remove possible tour start - swap the bottom element of the tour list */
			list_of_rand_start_points[rand_lookup[tour_index]] = list_of_rand_start_points[num_rand_start_points-1];
			rand_lookup[list_of_rand_start_points[num_rand_start_points-1]] = rand_lookup[tour_index];
			num_rand_start_points --;
			/* copy location into the child */
			if (swap == 0)
				genome_c1[tour_index] = genome_p1[tour_index];
			else
				genome_c1[tour_index] = genome_p2[tour_index];
			/* find the new spot in parent2 */
			tour2_idx = p2_lookup[genome_p1[tour_index]];
			/* copy in parent 2 since eventually this spot will be on tour */
			if (swap == 0)
				genome_c2[tour2_idx] = genome_p2[tour2_idx];
			else
				genome_c2[tour2_idx] = genome_p1[tour2_idx];
			/* update next tour spot */
			tour_index = tour2_idx;
		}

		swap = (swap + 1) % 2;
	}

	/* cleanup */
	free(p2_lookup);
	free(rand_lookup);
	free(list_of_rand_start_points);
}

/*---------------------------------------------------------------------------------------------
 * (function: xover_ox)
 * Davis 1985 - Applying Adaptive algorithms to Epistatic Domains
 *
 * p1 = 0, 1, 2, 3, 4
 * p2 = 1, 2, 4, 0, 3
 * cut = 2,3
 * c1 = 4, 0, 2, 3, 1
 * c1 = 2, 3, 4, 0, 1
 *-------------------------------------------------------------------------------------------*/
void xover_ox(void *gp1, void *gp2, void *gc1, void *gc2, int genome_size)
{
	int i;
	short *list_of_idx_to_gene_spots_to_not_copy1;
	short *list_of_idx_to_gene_spots_to_not_copy2;
	int temp;
	int swap_idx;

	int *genome_p1 = (int*)gp1;
	int *genome_p2 = (int*)gp2;
	int *genome_c1 = (int*)gc1;
	int *genome_c2 = (int*)gc2;

	/* cut point 1 needs to be in the first half */
	int cut_point1 = rand() % (genome_size/2);
	int cut_point2 = cut_point1 + (genome_size/2);

	list_of_idx_to_gene_spots_to_not_copy1 = (short*)calloc(genome_size, sizeof(short));
	list_of_idx_to_gene_spots_to_not_copy2 = (short*)calloc(genome_size, sizeof(short));

	for (i = cut_point1; i < cut_point2; i++)
	{
		genome_c1[i] = genome_p1[i];
		list_of_idx_to_gene_spots_to_not_copy1[genome_p1[i]] = TRUE;
		genome_c2[i] = genome_p2[i];
		list_of_idx_to_gene_spots_to_not_copy2[genome_p2[i]] = TRUE;
	}

	int idx1 = 0;
	int idx2 = 0;

	/* Note start after the cut with other parents nodes in order */
	for (i = cut_point2; i < genome_size; i++)
	{
		while (list_of_idx_to_gene_spots_to_not_copy1[genome_p2[idx1]] == TRUE)
		{
			idx1 ++;
		}
		genome_c1[i] = genome_p2[idx1];
		idx1 ++;

		while (list_of_idx_to_gene_spots_to_not_copy2[genome_p1[idx2]] == TRUE)
		{
			idx2 ++;
		}
		genome_c2[i] = genome_p1[idx2];
		idx2 ++;
	}

	/* finish up the 0 to cut_point1 of the child */
	for (i = 0; i < cut_point1; i++)
	{
		while (list_of_idx_to_gene_spots_to_not_copy1[genome_p2[idx1]] == TRUE)
		{
			idx1 ++;
		}
		genome_c1[i] = genome_p2[idx1];
		idx1 ++;

		while (list_of_idx_to_gene_spots_to_not_copy2[genome_p1[idx2]] == TRUE)
		{
			idx2 ++;
		}
		genome_c2[i] = genome_p1[idx2];
		idx2 ++;
	}

	/* cleanup */
	free(list_of_idx_to_gene_spots_to_not_copy1);
	free(list_of_idx_to_gene_spots_to_not_copy2);
}

/*---------------------------------------------------------------------------------------------
 * (function: xover_pmx)
 * Advancing genetic algorithm approaches to field programmable gate array placement with enhanced recombination operators Robert Collier, Christian Fobel, Ryan Pattison, Gary Grewal, Shawki Areibi, and Peter Jamieson. Journal of Evolutionary Intelligence. October 2014. 
 *-------------------------------------------------------------------------------------------*/
void confined_swap_recombination(void *gp1, void *gp2, void *gc1, void *gc2, int genome_size)
{
	int i, j;
	int *p1_lookup; // Algorithmically A
	int *p2_lookup; // Algorithmically B
	int *p3_lookup; // Algorithmically A2 so I can make 2 children
	int *p4_lookup; // Algorithmically B2 so I can make 2 children

	int *genome_p1 = (int*)gp1;
	int *genome_p2 = (int*)gp2;
	int *genome_c1 = (int*)gc1;
	int *genome_c2 = (int*)gc2;

	int x1, x2, y1, y2;
	int rand_int1;
	int rand_int2;
	int temp_idx1, temp_idx2;

	p1_lookup = (int*)malloc(sizeof(int) * genome_size);
	p2_lookup = (int*)malloc(sizeof(int) * genome_size);
	p3_lookup = (int*)malloc(sizeof(int) * genome_size);
	p4_lookup = (int*)malloc(sizeof(int) * genome_size);

	/* create the reverse lookup...as in if i want to know where city 3 is in genome p1 I can write p1_lookup[3] */
	for (j = 0; j < genome_size; j++)
	{
		genome_c1[j] = genome_p1[j];
		genome_c2[j] = genome_p2[j];
		/* Algorithm inverse lookup A = inverse(Q) */
		p1_lookup[genome_p1[j]] = j;
		/* Algorithm inverse lookup B = inverse(P') */
		p2_lookup[genome_p2[j]] = j;
		p3_lookup[genome_p1[j]] = j;
		p3_lookup[genome_p2[j]] = j;
	}

	/* now copy do X swaps where we will make X = to size of Genome */
	for (j = 0; j < genome_size; j++)
	{
		rand_int1 = rand() % genome_size;
		rand_int2 = rand() % genome_size;

		/* ALgorithm x = A[rand_int] y = B[rand_int] */
		x1 = p1_lookup[rand_int1];
		y1 = p2_lookup[rand_int1];
		x2 = p3_lookup[rand_int2];
		y2 = p4_lookup[rand_int2];

		/* Algorithm swap Q[x] with Q[y] */
		temp_idx1 = genome_c1[x1];
		temp_idx2 = genome_c2[x2];

		genome_c1[x1] = genome_c1[y1];
		genome_c2[x2] = genome_c2[y2];

		genome_c1[y1] = temp_idx1;
		genome_c2[y2] = temp_idx2;

		/* algorithm update A; A[Q[x]] = x */
		p1_lookup[genome_c1[x1]] = x1;
		p1_lookup[genome_c1[y1]] = y1;
		p2_lookup[genome_c2[x2]] = x2;
		p1_lookup[genome_c2[y2]] = y2;
	}

	/* cleanup */
	free(p1_lookup);
	free(p2_lookup);
	free(p3_lookup);
	free(p4_lookup);
}

