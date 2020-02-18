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

#include "tsp_globals.h"
#include "tsp_types.h"

#include "tsp_ga_genome_combinatorial_tupple_hw.h"

#include "genetic_algorithm_combinational_crossovers_hw.h"
#include "genetic_algorithm_selectors_hw.h"

#include "thread_utils.h"

tsp_problem_adjacency_hw_t adjacency;
int global_best_hw;
int global_index_hw;

/* FOR Hardware Implementation */
unsigned long int hw_next = 1;
 
void my_srand(int x)
{
	hw_next = x;
}
 
int my_rand(void) // RAND_MAX assumed to be 32767
{
    hw_next = hw_next * 1103515245 + 12345;
    return (unsigned int)(hw_next/65536) % 32768;
}


/*---------------------------------------------------------------------------------------------
 * (function: tsp_cost_function)
 *-------------------------------------------------------------------------------------------*/
int tsp_cost_function_from_adjacency_hw(int pop_idx, int toggle_idx)
{
	int i;
	int cost = 0;

	for (i = 1; i < GENOME_SIZE_HW; i++)
	{
		cost += adjacency.adjacency_matrix[ga_hw_data.population[toggle_idx][pop_idx][i]][ga_hw_data.population[toggle_idx][pop_idx][i-1]];
	}

	return cost;
}

int temp_pop[GENOME_SIZE_HW];
/*---------------------------------------------------------------------------------------------
void tsp_copy_genome_from_hw(int toggle_idx, int pop_idx)
 *-------------------------------------------------------------------------------------------*/
void tsp_copy_genome_from_hw(int toggle_idx, int pop_idx)
{
	int i; 

	for (i = 0; i < GENOME_SIZE_HW; i++)
	{
		temp_pop[i] = ga_hw_data.population[toggle_idx][pop_idx][i];
	}
}
/*---------------------------------------------------------------------------------------------
void tsp_copy_genome_to_hw(int toggle_idx, int pop_idx)
 *-------------------------------------------------------------------------------------------*/
void tsp_copy_genome_to_hw(int toggle_idx, int pop_idx)
{
	int i; 

	for (i = 0; i < GENOME_SIZE_HW; i++)
	{
		ga_hw_data.population[toggle_idx][pop_idx][i] = temp_pop[i];
	}
}
/*---------------------------------------------------------------------------------------------
void tsp_copy_genome_to_hw(int toggle_idx, int pop_idx)
 *-------------------------------------------------------------------------------------------*/
void tsp_copy_genomes_in_hw(int toggle_idx, int pop_idx_from, int pop_idx_to)
{
	int i; 

	for (i = 0; i < GENOME_SIZE_HW; i++)
	{
		ga_hw_data.population[toggle_idx][pop_idx_to][i] = ga_hw_data.population[toggle_idx][pop_idx_from][i];
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_cost_function_and_order_hw)
 *-------------------------------------------------------------------------------------------*/
void tsp_cost_function_and_order_hw(int toggle_idx)
{
	int i, j;
	int index_of_lowest_cost;

	for (i = 0; i < ga_hw_data.num_population; i++)
	{
		ga_hw_data.costs[i] = tsp_cost_function_from_adjacency_hw(i, toggle_idx);
	}

	/* Sort the list of populations */
	for (i = 0; i < ga_hw_data.num_population - 1; i++)
	{
		index_of_lowest_cost = i;
		int temp_cost;	
		for (j = i + 1; j < ga_hw_data.num_population; j ++)
		{
			if (ga_hw_data.costs[index_of_lowest_cost] > ga_hw_data.costs[j])
				index_of_lowest_cost = j;
		}

	    /* swap the top with new low */
        tsp_copy_genome_from_hw(global_index, i);
		temp_cost = ga_hw_data.costs[i];

		tsp_copy_genomes_in_hw(global_index, index_of_lowest_cost, i);
        ga_hw_data.costs[i] = ga_hw_data.costs[index_of_lowest_cost];

        tsp_copy_genome_to_hw(global_index, index_of_lowest_cost);
		ga_hw_data.costs[index_of_lowest_cost] = temp_cost;
	}

	printf("Best solution costs: %d\n", ga_hw_data.costs[0]);
	global_best_hw = ga_hw_data.costs[0];
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_copy_old_genomes)
 *-------------------------------------------------------------------------------------------*/
void tsp_copy_old_genomes_hw(int from_idx, int to_idx, int start, int end)
{
	int i;

	for (i = start; i < end; i++)
	{
		tsp_copy_solution_hw(from_idx, to_idx, i);
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_copy_solution)
 *-------------------------------------------------------------------------------------------*/
void tsp_copy_solution_hw(int from_idx, int to_idx, int idx)
{
	int i;

	for (i = 0; i < GENOME_SIZE_HW; i++)
	{
		ga_hw_data.population[to_idx][idx][i] = ga_hw_data.population[from_idx][idx][i];
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_copy_one_solution_hw)
 *-------------------------------------------------------------------------------------------*/
void tsp_copy_one_solution_hw(int from_idx, int f_idx, int to_idx, int t_idx)
{
	int i;

	for (i = 0; i < GENOME_SIZE_HW; i++)
	{
		ga_hw_data.population[to_idx][t_idx][i] = ga_hw_data.population[from_idx][f_idx][i];
	}
}
/*---------------------------------------------------------------------------------------------
 * (function: tsp_cross_breed)
 *-------------------------------------------------------------------------------------------*/
void tsp_cross_breed_hw(int toggle_idx, int start, int end)
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


	int temp_end = end-(end - start)%2; // make sure even number
	for (i = start; i < temp_end; i += 2)
	{
        /* pick up 2 parents */
        parent1 = selector_random_hw();
        parent2 = selector_random_hw();

		if (parent1 == parent2)
		{
			/* if two random numbers are the same */
			parent2 = (parent2 != ga_hw_data.num_population-1) ? parent2 + 1 : parent2 - 1;
		}

        xover_pmx_hw(toggle_idx, parent1, parent2, i, i+1, GENOME_SIZE_HW);
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_mutate)
 *-------------------------------------------------------------------------------------------*/
void tsp_mutate_hw(int toggle_idx, int start, int end, int from_best)
{
	int i, j;
	int who;
	int num_mutations = ga_hw_data.num_mutations;
	int swap_g1, swap_g2;
	int temp;

	for (i = start; i < end; i++)
	{
		/* find out who is the parent, copy over to "to" */
		who = my_rand() % from_best;
		tsp_copy_one_solution_hw(toggle_idx, who, global_index, i);

		/* mutate the copied genome */ 
		for (j = 0; j < num_mutations; j++)
		{
			swap_g1 = my_rand() % GENOME_SIZE_HW;
			swap_g2 = my_rand() % GENOME_SIZE_HW;
			temp = ga_hw_data.population[toggle_idx][i][swap_g1];
			ga_hw_data.population[toggle_idx][i][swap_g1] = ga_hw_data.population[toggle_idx][i][swap_g2];
			ga_hw_data.population[toggle_idx][i][swap_g2] = temp;
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_breed_and_mutate)
 *-------------------------------------------------------------------------------------------*/
void tsp_breed_and_mutate_hw( int toggle_idx, int start, int end)
{
	int i, j;
	int num_mutations = ga_hw_data.num_mutations;
	int swap_g1, swap_g2;
	int temp;

	/* do the cross breeding */
	tsp_cross_breed_hw(toggle_idx, start, end);

	/* mutate */
	for (i = start; i < end; i++)
	{
		/* mutate the copied genome */ 
		for (j = 0; j < num_mutations; j++)
		{
			swap_g1 = my_rand() % GENOME_SIZE_HW;
			swap_g2 = my_rand() % GENOME_SIZE_HW;
			temp = ga_hw_data.population[toggle_idx][i][swap_g1];
			ga_hw_data.population[toggle_idx][i][swap_g1] = ga_hw_data.population[toggle_idx][i][swap_g2];
			ga_hw_data.population[toggle_idx][i][swap_g2] = temp;
		}
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_random_new)
 *-------------------------------------------------------------------------------------------*/
void tsp_random_new_hw(int toggle_idx, int start, int end)
{
	int i, j;
	/* do num cities */
	int num_swaps;
	int temp;
	num_swaps = GENOME_SIZE_HW * 2;

	for (i = start; i < end; i++)
	{
		for (j = 0; j < num_swaps; j++)
		{
			int swap1 = my_rand()%GENOME_SIZE_HW;
			int swap2 = my_rand()%GENOME_SIZE_HW;
	
			temp = ga_hw_data.population[toggle_idx][i][swap1];
			ga_hw_data.population[toggle_idx][i][swap1] = ga_hw_data.population[toggle_idx][i][swap2];
			ga_hw_data.population[toggle_idx][i][swap2] = temp;
		}
	}

}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_exit_condition)
 *-------------------------------------------------------------------------------------------*/
short tsp_exit_condition_hw()
{
	static int breeding_cycles = 0;
	static int count = 0;
	static int recuring_best = 0;

	breeding_cycles ++;
	printf("breed cycle: %d\n", breeding_cycles);

#if 0
	if (breeding_cycles == 1000)
	{
		breeding_cycles = 0;
		return TRUE; // exit
	}
	else
	{
		return FALSE;
	}
#endif 

	if (global_best_hw <= recuring_best)
	{
		recuring_best = global_best_hw;
		count = 0;
	}
	else
	{
		count ++;
	}

	if (count > 500)
	{
		count = 0;
		breeding_cycles = 0;
		return TRUE; // exit
	}
	else
	{
		return FALSE;
	}
}
