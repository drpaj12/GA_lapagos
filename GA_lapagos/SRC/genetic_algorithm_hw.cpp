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

#include "genetic_algorithm.h"
#include "genetic_algorithm_combinational_crossovers.h"
#include "genetic_algorithm_bit_crossovers.h"

#include "tsp_ga_genome_combinatorial_tupple_hw.h"

/* Prototypes */

/* globals */
/*---------------------------------------------------------------------------------------------
 * (function: run_ga)
 *-------------------------------------------------------------------------------------------*/
void run_base_ga_hw()
{
	short exit = FALSE;
	short to_pop_idx;

	global_index_hw = 0;

	/* calculate indexes for new population */
	printf("idx_start_keep = %d\n", ga_hw_data.idx_start_keep);
	printf("idx_end_keep  = %d\n", ga_hw_data.idx_end_keep );
	printf("idx_start_mutate = %d\n", ga_hw_data.idx_start_mutate);
	printf("idx_end_mutate = %d\n", ga_hw_data.idx_end_mutate);
	printf("idx_start_breed = %d\n", ga_hw_data.idx_start_breed);
	printf("idx_end_breed = %d\n", ga_hw_data.idx_end_breed);
	printf("idx_start_breed_and_mutate = %d\n", ga_hw_data.idx_start_breed_and_mutate);
	printf("idx_end_breed_and_mutate = %d\n", ga_hw_data.idx_end_breed_and_mutate);
	printf("idx_start_random = %d\n", ga_hw_data.idx_start_random);
	printf("idx_end_random = %d\n", ga_hw_data.idx_end_random);
	printf("idx_end_breed_from	= %d\n", ga_hw_data.idx_end_breed_from );
	printf("idx_end_mutate_from = %d\n", ga_hw_data.idx_end_mutate_from);
	printf("idx_end_breed_mutate_from = %d\n", ga_hw_data.idx_end_breed_mutate_from);

	while (exit == FALSE)
	{
		to_pop_idx = (global_index_hw+1)%2;

		/* EVALUATE POPULATION AND ORDER */
		tsp_cost_function_and_order_hw(global_index_hw);

		/* KEEP OLD */
		tsp_copy_old_genomes_hw(global_index_hw, to_pop_idx, ga_hw_data.idx_start_keep, ga_hw_data.idx_end_keep);
	
		/* CROSSBREED NEW */
		tsp_cross_breed_hw(to_pop_idx, ga_hw_data.idx_start_breed, ga_hw_data.idx_end_breed);
	
		/* MUTATE NEW */
		tsp_mutate_hw(to_pop_idx, ga_hw_data.idx_start_mutate, ga_hw_data.idx_end_mutate, ga_hw_data.idx_end_mutate_from);

		/* CROSSBREED & MUTATE NEW */
		tsp_breed_and_mutate_hw(to_pop_idx, ga_hw_data.idx_start_breed_and_mutate, ga_hw_data.idx_end_breed_and_mutate);
	
		/* CREATE RANDOM NEW */
		tsp_random_new_hw(to_pop_idx, ga_hw_data.idx_start_random, ga_hw_data.idx_end_random);

		/* CHECK EXIT CONDITION */
		exit = tsp_exit_condition_hw();

		/* swap to the new population */
		global_index_hw = to_pop_idx;
	}
}
