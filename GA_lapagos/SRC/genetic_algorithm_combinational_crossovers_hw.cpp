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

#include "genetic_algorithm_combinational_crossovers_hw.h"
#include "genetic_algorithm_bit_crossovers.h"

/* Prototypes */

/* globals */
short list_of_idx_to_gene_spots_to_keep[GENOME_SIZE_HW];
short list_of_idx_to_gene_spots_to_keep2[GENOME_SIZE_HW];
int	p1_lookup[GENOME_SIZE_HW];
int	p2_lookup[GENOME_SIZE_HW];

/*---------------------------------------------------------------------------------------------
 * (function: setup_crossover_funcion)
 * 
 * A comprehensive listing and taxonomy of crossover operators.	 We implement a small section of these
 *
 * A Survey on Crossover Operators
 * G. PAVAI and T. V. GEETHA, Anna University
 *-------------------------------------------------------------------------------------------*/
void crossover_function_hw(int toggle_idx, int gp1, int gp2, int gc1, int gc2, int genome_size)
{
	switch(ga_hw_data.crossover_type)
	{
		case PMX:
			xover_pmx_hw(toggle_idx, gp1, gp2, gc1, gc2, genome_size);
			break;
		case POSITION:
			xover_position_hw(toggle_idx, gp1, gp2, gc1, gc2, genome_size);
			break;
		case CX:
			xover_cx_hw(toggle_idx, gp1, gp2, gc1, gc2, genome_size);
			break;
		case CX2:
			xover_cx2_hw(toggle_idx, gp1, gp2, gc1, gc2, genome_size);
			break;
		case OX:
			xover_ox_hw(toggle_idx, gp1, gp2, gc1, gc2, genome_size);
			break;
		case SINGLE_POINT:
			printf("Unsupported for HW\n");
			//xover_single_point_hw(toggle_idx, gp1, gp2, gc1, gc2, genome_size);
			break;
		case TWO_POINT:
			printf("Unsupported for HW\n");
			//xover_two_point_hw(toggle_idx, gp1, gp2, gc1, gc2, genome_size);
			break;
		case UNIFORM:
			//xover_uniform_hw(toggle_idx, gp1, gp2, gc1, gc2, genome_size);
			printf("Unsupported for HW\n");
			break;
		default:
			printf("Not recognized crossover operator\n");
			return;
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: xover_pmx)
 * Goldberg 1985 "Alleles, loci, and the Travelling saleman problem"
 *-------------------------------------------------------------------------------------------*/
void xover_pmx_hw(int toggle_idx, int gp1, int gp2, int gc1, int gc2, int genome_size)
{
	int i, j;
	int temp;
	int swap_idx;

	/* cut point 1 needs to be in the first half */
	int cut_point1 = rand() % (genome_size/2);
	int cut_point2 = cut_point1 + (genome_size/2);

	for (i = 0; i < genome_size; i++)
	{
		if (i > cut_point1 || i < cut_point2)
		{
			list_of_idx_to_gene_spots_to_keep[i] = TRUE;
			//genome_c1[i] = genome_p1[i];
			ga_hw_data.population[toggle_idx][gc1][i] = ga_hw_data.population[toggle_idx][gp1][i]; 
			//genome_c2[i] = genome_p2[i];
			ga_hw_data.population[toggle_idx][gc2][i] = ga_hw_data.population[toggle_idx][gp2][i]; 
		}
		else
			list_of_idx_to_gene_spots_to_keep[i] = FALSE;
	}

	/* create the reverse lookup...as in if i want to know where city 3 is in genome p1 I can write p1_lookup[3] */
	for (j = 0; j < genome_size; j++)
	{
		//p1_lookup[genome_p1[j]] = j;
		p1_lookup[ga_hw_data.population[toggle_idx][gp1][j]] = j;
		//p2_lookup[genome_p2[j]] = j;
		p2_lookup[ga_hw_data.population[toggle_idx][gp2][j]] = j;
	}

	/* now copy over the parents details with a remapping for overlaps */
	for (j = 0; j < genome_size; j++)
	{
		/* copy c1 from p2 */
		if (list_of_idx_to_gene_spots_to_keep[j] == FALSE)
		{
			//int genome_id_for_here = genome_p2[j];
			int genome_id_for_here = ga_hw_data.population[toggle_idx][gp2][j];
			/* If this is not true then get from parent2 */
			while (list_of_idx_to_gene_spots_to_keep[p1_lookup[genome_id_for_here]] == TRUE)
			{
				//genome_id_for_here = genome_p2[p1_lookup[genome_id_for_here]];
				genome_id_for_here = ga_hw_data.population[toggle_idx][gp2][p1_lookup[genome_id_for_here]];
			}
			//genome_c1[j] = genome_id_for_here;
			ga_hw_data.population[toggle_idx][gc1][j] = genome_id_for_here;
		}

		/* copy c2 from p1 */
		if (list_of_idx_to_gene_spots_to_keep[j] == FALSE)
		{
			//int genome_id_for_here = genome_p1[j];
			int genome_id_for_here = ga_hw_data.population[toggle_idx][gp1][j];
			/* If this is not true then get from parent2 */
			while (list_of_idx_to_gene_spots_to_keep[p2_lookup[genome_id_for_here]] == TRUE)
			{
				//genome_id_for_here = genome_p1[p2_lookup[genome_id_for_here]];
				genome_id_for_here = ga_hw_data.population[toggle_idx][gp1][p2_lookup[genome_id_for_here]];
			}
			//genome_c2[j] = genome_id_for_here;
			ga_hw_data.population[toggle_idx][gc2][j] = genome_id_for_here;
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: xover_position)
 * Syswerda 1990 "Schedule Optimization Using Genetic Algorithms"
 * PMX like but random set is chosen and then mapping done after
 *-------------------------------------------------------------------------------------------*/
void xover_position_hw(int toggle_idx, int gp1, int gp2, int gc1, int gc2, int genome_size)
{
	int i, j;
	int temp;
	int swap_idx;

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
		//p1_lookup[genome_p1[j]] = j;
		p1_lookup[ga_hw_data.population[toggle_idx][gp1][j]] = j;
		//p2_lookup[genome_p2[j]] = j;
		p2_lookup[ga_hw_data.population[toggle_idx][gp2][i]] = j;
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
			//int genome_id_for_here = genome_p2[j];
			int genome_id_for_here = ga_hw_data.population[toggle_idx][gp2][j];
			/* If this is not true then get from parent2 */
			while (list_of_idx_to_gene_spots_to_keep[p1_lookup[genome_id_for_here]] == TRUE)
			{
				//genome_id_for_here = genome_p2[p1_lookup[genome_id_for_here]];
				genome_id_for_here = ga_hw_data.population[toggle_idx][gp2][p1_lookup[genome_id_for_here]];
			}
			//genome_c1[j] = genome_id_for_here;
			ga_hw_data.population[toggle_idx][gc1][j] = genome_id_for_here;
		}
		else
		{
			//genome_c1[j] = genome_p1[j];
			ga_hw_data.population[toggle_idx][gc1][j] = ga_hw_data.population[toggle_idx][gp1][j];
		}	

		/* copy c2 from p1 */
		if (list_of_idx_to_gene_spots_to_keep[j] == FALSE)
		{
			//int genome_id_for_here = genome_p1[j];
			int genome_id_for_here = ga_hw_data.population[toggle_idx][gp1][j];
			/* If this is not true then get from parent2 */
			while (list_of_idx_to_gene_spots_to_keep[p2_lookup[genome_id_for_here]] == TRUE)
			{
				//genome_id_for_here = genome_p1[p2_lookup[genome_id_for_here]];
				genome_id_for_here = ga_hw_data.population[toggle_idx][gp1][p2_lookup[genome_id_for_here]];
			}
			//genome_c2[j] = genome_id_for_here;
			ga_hw_data.population[toggle_idx][gc2][j] = genome_id_for_here;
		}
		else
		{
			//genome_c2[j] = genome_p2[j];
			ga_hw_data.population[toggle_idx][gc2][j] = ga_hw_data.population[toggle_idx][gp2][j];
		}	
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: xover_cx)
 * Oliver 1987 "A Study of Permutation Crossover on the Travelling Salesman Problem"
 * Cycle crossover function (also known as CX) on the int permutations
 * proposed by Oliver [15] builds offspring in such a way that each city (and its position) 
 * comes from one of the parents
 * Note: One cycle is made, and then all the other data is copied from the opposite parent.	 
 *-------------------------------------------------------------------------------------------*/
void xover_cx_hw(int toggle_idx, int gp1, int gp2, int gc1, int gc2, int genome_size)
{
	int i;
	int temp;
	int swap_idx;

	int tour_index;
	int start_tour_index;
	int tour2_idx;

	/* create the reverse lookup...as in if i want to know where city 3 is in genome p1 I can write p1_lookup[3] */
	for (i = 0; i < genome_size; i++)
	{
		list_of_idx_to_gene_spots_to_keep[i] = 0;
		//p2_lookup[genome_p2[i]] = i;
		p2_lookup[ga_hw_data.population[toggle_idx][gp2][i]] = i;
	}

	/* pick a random spot to start tour */
	tour_index = rand() % genome_size;

	start_tour_index = tour_index;
	/* mark spot done */
	list_of_idx_to_gene_spots_to_keep[tour_index] = TRUE;
	/* copy location into the child */
	//genome_c1[tour_index] = genome_p1[tour_index];
	ga_hw_data.population[toggle_idx][gc1][tour_index] = ga_hw_data.population[toggle_idx][gp1][tour_index];
	/* find the new spot in parent2 */
	//tour2_idx = p2_lookup[genome_p1[tour_index]];
	tour2_idx = p2_lookup[ga_hw_data.population[toggle_idx][gp1][tour_index]];
	/* copy in parent 2 since eventually this spot will be on tour */
	//genome_c2[tour2_idx] = genome_p2[tour2_idx];
	ga_hw_data.population[toggle_idx][gc2][tour2_idx] = ga_hw_data.population[toggle_idx][gp2][tour2_idx];
	/* update next tour spot */
	tour_index = tour2_idx;

	while (tour_index != start_tour_index)
	{
		/* mark spot done */
		list_of_idx_to_gene_spots_to_keep[tour_index] = TRUE;
		/* copy location into the child */
		//genome_c1[tour_index] = genome_p1[tour_index];
		ga_hw_data.population[toggle_idx][gc1][tour_index] = ga_hw_data.population[toggle_idx][gp1][tour_index];
		/* find the new spot in parent2 */
		//tour2_idx = p2_lookup[genome_p1[tour_index]];
		tour2_idx = p2_lookup[ga_hw_data.population[toggle_idx][gp1][tour_index]];
		/* copy in parent 2 since eventually this spot will be on tour */
		//genome_c2[tour2_idx] = genome_p2[tour2_idx];
		ga_hw_data.population[toggle_idx][gc2][tour2_idx] = ga_hw_data.population[toggle_idx][gp2][tour2_idx];
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
			//genome_c1[i] = genome_p2[i];
			ga_hw_data.population[toggle_idx][gc1][i] = ga_hw_data.population[toggle_idx][gp2][i];
			
			/* If this is not true then get from parent1 */
			//genome_c2[i] = genome_p1[i];
			ga_hw_data.population[toggle_idx][gc2][i] = ga_hw_data.population[toggle_idx][gp1][i];
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: xover_cx2)
 * Extension of cx to make all cycles
 * Hussain 2017 Genetic Algorithm for Traveling Salesman Problem with Modified Cycle Crossover Operator
 *-------------------------------------------------------------------------------------------*/
void xover_cx2_hw(int toggle_idx, int gp1, int gp2, int gc1, int gc2, int genome_size)
{
	int i;
	int num_rand_start_points;
	int rand_start_point;
	// int *list_of_idx_to_gene_spots_to_keep; /* int *list_of_rand_start_points; */
	// int *p1_lookup; // int *rand_lookup;

	int tour_index;
	int start_tour_index;
	int tour2_idx;

	short swap = 0;

	/* create the reverse lookup...as in if i want to know where city 3 is in genome p1 I can write p1_lookup[3] */
	for (i = 0; i < genome_size; i++)
	{
		//p2_lookup[genome_p2[i]] = i;
		p2_lookup[ga_hw_data.population[toggle_idx][gp2][i]] = i;
		list_of_idx_to_gene_spots_to_keep[i] = i;
		p1_lookup[i] = i;
	}

	/* set the list of rand points */
	num_rand_start_points = genome_size;

	while (num_rand_start_points > 0)
	{
		rand_start_point = rand() % num_rand_start_points;
		/* pick a random spot to start tour */
		tour_index = list_of_idx_to_gene_spots_to_keep[rand_start_point];

		/* remove possible tour start - swap the bottom element of the tour list */
		list_of_idx_to_gene_spots_to_keep[p1_lookup[tour_index]] = list_of_idx_to_gene_spots_to_keep[num_rand_start_points-1];
		p1_lookup[list_of_idx_to_gene_spots_to_keep[num_rand_start_points-1]] = p1_lookup[tour_index];
		num_rand_start_points --;
	
		start_tour_index = tour_index;

		/* copy location into the child */
		if (swap == 0)
			//genome_c1[tour_index] = genome_p1[tour_index];
			ga_hw_data.population[toggle_idx][gc1][tour_index] = ga_hw_data.population[toggle_idx][gp1][tour_index];
		else
			//genome_c1[tour_index] = genome_p2[tour_index];
			ga_hw_data.population[toggle_idx][gc1][tour_index] = ga_hw_data.population[toggle_idx][gp2][tour_index];

		/* find the new spot in parent2 */
		//tour2_idx = p2_lookup[genome_p1[tour_index]];
		tour2_idx = p2_lookup[ga_hw_data.population[toggle_idx][gp1][tour_index]];
		/* copy in parent 2 since eventually this spot will be on tour */
		if (swap == 0)
			//genome_c2[tour2_idx] = genome_p2[tour2_idx];
			ga_hw_data.population[toggle_idx][gc2][tour2_idx] = ga_hw_data.population[toggle_idx][gp2][tour2_idx];
		else
			//genome_c2[tour2_idx] = genome_p1[tour2_idx];
			ga_hw_data.population[toggle_idx][gc2][tour2_idx] = ga_hw_data.population[toggle_idx][gp1][tour2_idx];
		/* update next tour spot */
		tour_index = tour2_idx;
	
		while (tour_index != start_tour_index)
		{
			/* remove possible tour start - swap the bottom element of the tour list */
			list_of_idx_to_gene_spots_to_keep[p1_lookup[tour_index]] = list_of_idx_to_gene_spots_to_keep[num_rand_start_points-1];
			p1_lookup[list_of_idx_to_gene_spots_to_keep[num_rand_start_points-1]] = p1_lookup[tour_index];
			num_rand_start_points --;
			/* copy location into the child */
			if (swap == 0)
				//genome_c1[tour_index] = genome_p1[tour_index];
				ga_hw_data.population[toggle_idx][gc1][tour_index] = ga_hw_data.population[toggle_idx][gp1][tour_index];
			else
				//genome_c1[tour_index] = genome_p2[tour_index];
				ga_hw_data.population[toggle_idx][gc1][tour_index] = ga_hw_data.population[toggle_idx][gp2][tour_index];
			/* find the new spot in parent2 */
			//tour2_idx = p2_lookup[genome_p1[tour_index]];
			tour2_idx = p2_lookup[ga_hw_data.population[toggle_idx][gp1][tour_index]];
			/* copy in parent 2 since eventually this spot will be on tour */
			if (swap == 0)
				//genome_c2[tour2_idx] = genome_p2[tour2_idx];
				ga_hw_data.population[toggle_idx][gc2][tour2_idx] = ga_hw_data.population[toggle_idx][gp2][tour2_idx];
			else
				//genome_c2[tour2_idx] = genome_p1[tour2_idx];
				ga_hw_data.population[toggle_idx][gc2][tour2_idx] = ga_hw_data.population[toggle_idx][gp1][tour2_idx];
			/* update next tour spot */
			tour_index = tour2_idx;
		}

		swap = (swap + 1) % 2;
	}
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
void xover_ox_hw(int toggle_idx, int gp1, int gp2, int gc1, int gc2, int genome_size)
{
	int i;
	/* short *list_of_idx_to_gene_spots_to_keep; actually not_to_copy */
	int temp;
	int swap_idx;

	/* cut point 1 needs to be in the first half */
	int cut_point1 = rand() % (genome_size/2);
	int cut_point2 = cut_point1 + (genome_size/2);

	for (i = 0; i < genome_size; i++)
	{
		list_of_idx_to_gene_spots_to_keep[i] = 0;
		list_of_idx_to_gene_spots_to_keep2[i] = 0;
	}

	for (i = cut_point1; i < cut_point2; i++)
	{
		//genome_c1[i] = genome_p1[i];
		ga_hw_data.population[toggle_idx][gc1][i] = ga_hw_data.population[toggle_idx][gp1][i];
		//list_of_idx_to_gene_spots_to_keep[genome_p1[i]] = TRUE;
		list_of_idx_to_gene_spots_to_keep[ga_hw_data.population[toggle_idx][gp1][i]] = TRUE;
		//genome_c2[i] = genome_p2[i];
		ga_hw_data.population[toggle_idx][gc2][i] = ga_hw_data.population[toggle_idx][gp2][i];
		//list_of_idx_to_gene_spots_to_keep2[genome_p2[i]] = TRUE;
		list_of_idx_to_gene_spots_to_keep2[ga_hw_data.population[toggle_idx][gp2][i]] = TRUE;
	}

	int idx1 = 0;
	int idx2 = 0;

	/* Note start after the cut with other parents nodes in order */
	for (i = cut_point2; i < genome_size; i++)
	{
		//while (list_of_idx_to_gene_spots_to_keep[genome_p2[idx1]] == TRUE)
		while (list_of_idx_to_gene_spots_to_keep[ga_hw_data.population[toggle_idx][gp2][idx1]] == TRUE)
		{
			idx1 ++;
		}
		//genome_c1[i] = genome_p2[idx1];
		ga_hw_data.population[toggle_idx][gc1][i] = ga_hw_data.population[toggle_idx][gp2][idx1];
		idx1 ++;

		//while (list_of_idx_to_gene_spots_to_keep2[genome_p1[idx2]] == TRUE)
		while (list_of_idx_to_gene_spots_to_keep2[ga_hw_data.population[toggle_idx][gp1][idx2]] == TRUE)
		{
			idx2 ++;
		}
		//genome_c2[i] = genome_p1[idx2];
		ga_hw_data.population[toggle_idx][gc2][i] = ga_hw_data.population[toggle_idx][gp1][idx2];
		idx2 ++;
	}

	/* finish up the 0 to cut_point1 of the child */
	for (i = 0; i < cut_point1; i++)
	{
		//while (list_of_idx_to_gene_spots_to_keep[genome_p2[idx1]] == TRUE)
		while (list_of_idx_to_gene_spots_to_keep[ga_hw_data.population[toggle_idx][gp2][idx1]] == TRUE)
		{
			idx1 ++;
		}
		//genome_c1[i] = genome_p2[idx1];
		ga_hw_data.population[toggle_idx][gc1][i] = ga_hw_data.population[toggle_idx][gp2][idx1];
		idx1 ++;

		//while (list_of_idx_to_gene_spots_to_keep2[genome_p1[idx2]] == TRUE)
		while (list_of_idx_to_gene_spots_to_keep2[ga_hw_data.population[toggle_idx][gp1][idx2]] == TRUE)
		{
			idx2 ++;
		}
		//genome_c2[i] = genome_p1[idx2];
		ga_hw_data.population[toggle_idx][gc2][i] = ga_hw_data.population[toggle_idx][gp1][idx2];
		idx2 ++;
	}
}
