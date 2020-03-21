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

#include "genetic_algorithm.h"
#include "genetic_algorithm_combinational_crossovers.h"
#include "genetic_algorithm_selectors.h"

#include "mkp_globals.h"
#include "mkp_types.h"

#include "mkp_ga_genome_binary.h"
#include "mkp_dat.h"

/* Prototypes */

mkp_problem_t mkp_problem;
const char *mkp_problem_types_name[] = {	"DAT_FORMAT_PENALTY",
											"DAT_FORMAT_FIX" 
									   };

/*-------------------------------------------------------------------------
 * (function: mkp_setup_problem)
 *-----------------------------------------------------------------------*/
void mkp_setup_problem()
{
	mkp_problem.problem_type = (enum MKP_PROBLEM_TYPES)return_string_in_list(configuration.problem_type, (char**)mkp_problem_types_name, NUM_MKP_PROBLEM_TYPES);

	mkp_problem.num_variables = 0;

	switch(mkp_problem.problem_type)
	{
		case DAT_FORMAT_PENALTY:
		case DAT_FORMAT_FIX:
			/* read the benchmark xml in */
			read_mkp_problem_dat(configuration.benchmark_file_name);
			break;
		default:
			printf("Not recognized mkp problem type!!!\n");
	}
}

/*-------------------------------------------------------------------------
 * (function: mkp_free_problem)
 *-----------------------------------------------------------------------*/
void mkp_free_problem()
{
	switch(mkp_problem.problem_type)
	{
		case DAT_FORMAT_PENALTY:
		case DAT_FORMAT_FIX:
			/* free data structures */
			free_mkp_problem_dat();
			break;
		default:
			printf("Not recognized mkp problem type!!!\n");
	}
}

/*-------------------------------------------------------------------------
 * (function: mkp_run_ga)
 *-----------------------------------------------------------------------*/
void mkp_run_ga()
{
	double (*fptr_cost_function)(void *);
	   
	switch(mkp_problem.problem_type)
	{
		case DAT_FORMAT_PENALTY:
			fptr_cost_function = mkp_cost_function_penalty;
			break;
		case DAT_FORMAT_FIX:
			fptr_cost_function = mkp_cost_function_fix;
			break;
		default:
			printf("Not recognized mkp problem type!!!\n");
			return;
	}

	run_base_ga(
		(int (*)())setup_selector_function(),
		(void (*)(int))setup_selector_init_function(),
		mkp_cost_function_and_order,
		fptr_cost_function,
		mkp_copy_old_genomes,
		mkp_cross_breed,
		mkp_mutate,
		mkp_breed_and_mutate,
		mkp_random_new,
		mkp_cost_report_best,
		(void (*)(void*,void*,void*,void*,int))setup_crossover_function(),
		mkp_exit_condition);

	/* TEST OUT REPORT */
	output_test_details((*fptr_cost_function)(genomes.population[global_index][0]->genome));
}
