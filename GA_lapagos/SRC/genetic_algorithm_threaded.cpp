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
#include <pthread.h>
#include "globals.h"
#include "types.h"

#include "genetic_algorithm.h"
#include "genetic_algorithm_combinational_crossovers.h"
#include "genetic_algorithm_bit_crossovers.h"
#include "thread_utils.h"

/* Prototypes */

/* globals */
short thread_exit = FALSE;
short thread_to_pop_idx;

/*---------------------------------------------------------------------------------------------
 * (function: run_ga)
 *-------------------------------------------------------------------------------------------*/
int run_base_ga_threaded(
        thread_t *thread_struct,
        int (*fptr_selector)(),
        void (*fptr_selector_init)(int),
		void (*fptr_cost_function_order)(thread_t *, double (*)(void *)),
		double (*fptr_cost_function)(void *),
		void (*fptr_copy_old_over)(thread_t *, population_t**, population_t**, int, int),
		void (*fptr_cross_breed)(
                                        thread_t *,
                                        void (*)(void *, void *, void *, void *, int), 
                                        int (*)(),
                                        void (*)(int),
                                        population_t**, population_t**, int, int),
		void (*fptr_mutate)(thread_t *, population_t**, population_t**, int, int, int),
		void (*fptr_cross_and_mutate)(
                                        thread_t *,
                                        void (*)(void *, void *, void *, void *, int), 
                                        int (*)(),
                                        void (*)(int),
                                        population_t**, population_t**, int, int),
		void (*fptr_random_new)(thread_t *, population_t**, int, int),
		void (*fptr_cost_report_best)(FILE *, double (*)(void *)),
        void (*fptr_crossover)(void *, void *, void *, void *, int),
		short (*fptr_exit_condition)(void)
		)
{

	global_index = 0;

	/* calculate indexes for new population */
	int idx_start_keep = 0;
	int idx_end_keep = (int)floor(genomes.num_population * genomes.percent_to_keep);
	int idx_start_mutate = idx_end_keep;
	int idx_end_mutate = idx_end_keep + (int)(floor(genomes.num_population * genomes.percent_from_mutate));
	int idx_start_breed = idx_end_mutate;
	int idx_end_breed = idx_end_mutate + (int)(floor(genomes.num_population * genomes.percent_from_breed));
	int idx_start_breed_and_mutate = idx_end_breed;
	int idx_end_breed_and_mutate = idx_end_breed + (int)(floor(genomes.num_population * genomes.percent_from_breed_and_mutate));
	int idx_start_random = idx_end_breed_and_mutate;
	int idx_end_random = idx_end_breed_and_mutate + (int)(floor(genomes.num_population * genomes.percent_to_random));

	int idx_end_breed_from = (int)(floor(genomes.num_population * genomes.top_percent_to_breed_from));
	int idx_end_mutate_from = (int)(floor(genomes.num_population * genomes.top_percent_to_mutate_from));
	int idx_end_breed_mutate_from = (int)(floor(genomes.num_population * genomes.top_percent_to_breed_mutate_from));

    if (thread_struct->id == 0)
    {
	    printf("idx_start_keep = %d\n", idx_start_keep);
	    printf("idx_end_keep  = %d\n", idx_end_keep );
	    printf("idx_start_mutate = %d\n", idx_start_mutate);
	    printf("idx_end_mutate = %d\n", idx_end_mutate);
	    printf("idx_start_breed = %d\n", idx_start_breed);
	    printf("idx_end_breed = %d\n", idx_end_breed);
	    printf("idx_start_breed_and_mutate = %d\n", idx_start_breed_and_mutate);
	    printf("idx_end_breed_and_mutate = %d\n", idx_end_breed_and_mutate);
	    printf("idx_start_random = %d\n", idx_start_random);
	    printf("idx_end_random = %d\n", idx_end_random);
	    printf("idx_end_breed_from  = %d\n", idx_end_breed_from );
	    printf("idx_end_mutate_from = %d\n", idx_end_mutate_from);
	    printf("idx_end_breed_mutate_from = %d\n", idx_end_breed_mutate_from);
    }

	while (thread_exit == FALSE)
	{
        if (thread_struct->id == 0)
        {
    		thread_to_pop_idx = (global_index+1)%2;

            /* Wait for everyone so we have the right variables */
            barrier(thread_struct);
        }
        else
        {
            /* Wait for everyone to get to next gen */
            barrier(thread_struct);
        }

		/* EVALUATE POPULATION AND ORDER */
		(*fptr_cost_function_order)(
                                    thread_struct,
                                    (*fptr_cost_function));

        /* Wait for everyone - so list is ordered properly */
        barrier(thread_struct);

		/* KEEP OLD */
		(*fptr_copy_old_over)(thread_struct, genomes.population[global_index], genomes.population[thread_to_pop_idx], idx_start_keep, idx_end_keep);
	
		/* CROSSBREED NEW */
		(*fptr_cross_breed)(
                            thread_struct,
                            (*fptr_crossover),
                            (*fptr_selector),
                            (*fptr_selector_init),
                            genomes.population[global_index], 
                            genomes.population[thread_to_pop_idx], 
                            idx_start_breed, 
                            idx_end_breed);
	
		/* MUTATE NEW */
		(*fptr_mutate)(thread_struct, genomes.population[global_index], genomes.population[thread_to_pop_idx], idx_start_mutate, idx_end_mutate, idx_end_mutate_from);

		/* CROSSBREED & MUTATE NEW */
		(*fptr_cross_and_mutate)(
                                    thread_struct,
                                    (*fptr_crossover),
                                    (*fptr_selector),
                                    (*fptr_selector_init),
                                    genomes.population[global_index], 
                                    genomes.population[thread_to_pop_idx], 
                                    idx_start_breed_and_mutate, 
                                    idx_end_breed_and_mutate);
	
		/* CREATE RANDOM NEW */
		(*fptr_random_new)(thread_struct, genomes.population[thread_to_pop_idx], idx_start_random, idx_end_random);

		if (thread_struct->id == 0)
        {
    		/* CHECK EXIT CONDITION */
    		thread_exit = (*fptr_exit_condition)();

            /* wait for everyone to finish making the next gen */
            barrier(thread_struct);

            (*fptr_cost_report_best)(
                                fresult_out, 
                                (*fptr_cost_function));

		    /* swap to the new population */
    		global_index = thread_to_pop_idx;
        }
        else
        {
            /* Wait for everyone */
            barrier(thread_struct);
        }
	}
}
