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

#include "mkp_globals.h"
#include "mkp_types.h"
#include "mkp_dat.h"

/* Prototypes */

/* globals */

/*-------------------------------------------------------------------------
 * (function: read_mkp_benchmark_dat)
 * benchmarks from - http://www.eecs.qmul.ac.uk/~jdrake/mkp.html
 * --- format
 * #Variables (n), #Constraints (m), Optimal value (0 if unavailable)
 * Profit P(j) for each n
 * n x m matrix of constraints
 * Capacity b(i) for each m
 *-----------------------------------------------------------------------*/
void read_mkp_problem_dat(char *benchmark_name)
{
	FILE *f_benchmark;
	int i,j;

	f_benchmark = fopen(benchmark_name, "r");

	/* read n */
	fscanf(f_benchmark, "%d", &mkp_problem.num_variables);

	/* read m */
	fscanf(f_benchmark, "%d", &mkp_problem.num_constraints);

	/* read optimal value */
	fscanf(f_benchmark, "%d", &mkp_problem.optimal);

	/* allocate and read profits */
	mkp_problem.profit = (int*)malloc(sizeof(int)*mkp_problem.num_variables);
	for (i = 0; i < mkp_problem.num_variables; i++)
	{
		fscanf(f_benchmark, "%d", &mkp_problem.profit[i]);
	}
	
	/* allocate and read resource matrix */
	mkp_problem.resource_matrix = (int**)malloc(sizeof(int*)*mkp_problem.num_constraints);
	for (i = 0; i < mkp_problem.num_constraints; i++)
	{
		mkp_problem.resource_matrix[i] = (int*)malloc(sizeof(int)*mkp_problem.num_variables);
		for (j = 0; j < mkp_problem.num_variables; j++)
		{
			fscanf(f_benchmark, "%d", &mkp_problem.resource_matrix[i][j]); 
		}
	}

	/* allocate and read bounds */
	mkp_problem.bounds = (int*)malloc(sizeof(int)*mkp_problem.num_constraints);
	for (i = 0; i < mkp_problem.num_constraints; i++)
	{
		fscanf(f_benchmark, "%d", &mkp_problem.bounds[i]);
	}
 
	fclose(f_benchmark);

	return;
}

/*-------------------------------------------------------------------------
 * (function: free_configuartion)
 *-----------------------------------------------------------------------*/
void free_mkp_problem_dat()
{
	int i;

	if (mkp_problem.num_variables != 0)
	{
		free(mkp_problem.profit);
		
		for (i = 0; i < mkp_problem.num_constraints; i++)
		{
			free(mkp_problem.resource_matrix[i]);
		}
		free(mkp_problem.resource_matrix);
	
		free(mkp_problem.bounds);
	}
}


/*---------------------------------------------------------------------------------------------
 * (function: mkp_cost_function_penalty)
 *-------------------------------------------------------------------------------------------*/
double mkp_cost_function_penalty(void *genome)
{
	int i, j;
	double profit = 0.0;
	double temp_profit = 0.0;
	double *satisfying;
	bstr x_solution_bits = (bstr)genome;

	/* calculate profit */
	for (i = 0; i < mkp_problem.num_variables; i++)
	{
		profit += bitstr_test(x_solution_bits, i) * mkp_problem.profit[i];
	}

	temp_profit = profit/mkp_problem.num_constraints;

	satisfying = (double*)malloc(sizeof(double)*mkp_problem.num_constraints);
	/* check constraints */
	for (i = 0; i < mkp_problem.num_constraints; i++)
	{
		satisfying[i] = 0.0;

		for (j = 0; j < mkp_problem.num_variables; j++) 
		{
			satisfying[i] += bitstr_test(x_solution_bits, j) * mkp_problem.resource_matrix[i][j];
		}

		if (satisfying[i] > mkp_problem.bounds[i])
		{
			profit -= temp_profit;
		}
	}

	free(satisfying);

	return profit;
}

/*---------------------------------------------------------------------------------------------
 * (function: mkp_verify_satisfiability)
 *-------------------------------------------------------------------------------------------*/
void mkp_verify_satisfiability(void *genome)
{
	int i, j;
	double *satisfying;
	bstr x_solution_bits = (bstr)genome;

	satisfying = (double*)malloc(sizeof(double)*mkp_problem.num_constraints);
	/* check constraints */
	for (i = 0; i < mkp_problem.num_constraints; i++)
	{
		satisfying[i] = 0.0;

		for (j = 0; j < mkp_problem.num_variables; j++) 
		{
			satisfying[i] += bitstr_test(x_solution_bits, j) * mkp_problem.resource_matrix[i][j];
		}

		if (satisfying[i] > mkp_problem.bounds[i])
		{
			printf("Unsatisfied %f of bound[%d] %d\n", satisfying[i], i, mkp_problem.bounds[i]); 
		}
		else
		{
			printf("Satisfied %f of bound[%d] %d\n", satisfying[i], i, mkp_problem.bounds[i]); 
		}
	}

	free(satisfying);
}

/*---------------------------------------------------------------------------------------------
 * (function: mkp_cost_function_with_fix)
 *-------------------------------------------------------------------------------------------*/
double mkp_cost_function_fix(void *genome)
{
	int i, j, k;
	double profit = 0.0;
	double *satisfying;
	bstr x_solution_bits = (bstr)genome;
	short satisfied;
	int idx_j;

	satisfying = (double*)malloc(sizeof(double)*mkp_problem.num_constraints);
	/* check constraints */
	for (i = 0; i < mkp_problem.num_constraints; i++)
	{
		satisfying[i] = 0.0;

		for (j = 0; j < mkp_problem.num_variables; j++) 
		{
			satisfying[i] += bitstr_test(x_solution_bits, j) * mkp_problem.resource_matrix[i][j];
		}

	}

	/* fix algorithm */
	/* DROP values to make satisfiable */
	for (i = 0; i < mkp_problem.num_constraints; i++)
	{
		idx_j = 0;

		while (satisfying[i] > mkp_problem.bounds[i])
		{
			if (bitstr_test(x_solution_bits, idx_j))
			{
				for (k = 0; k < mkp_problem.num_constraints; k++)
					satisfying[k] -= mkp_problem.resource_matrix[k][idx_j];
				bitstr_clear(x_solution_bits, idx_j);
			}
			idx_j++;
		}
	}
	/* ADD values but keep satisfiable */
	for (j = 0; j < mkp_problem.num_variables; j++)
	{
		satisfied = TRUE;

		if (bitstr_test(x_solution_bits, j) == FALSE)
		{
			for (k = 0; k < mkp_problem.num_constraints; k++)
			{
				if (satisfying[k] + mkp_problem.resource_matrix[k][j] > mkp_problem.bounds[k])
				{
					satisfied = FALSE;
					break;
				}
			}

			if (satisfied == TRUE)
			{
				bitstr_set(x_solution_bits, j);
				for (k = 0; k < mkp_problem.num_constraints; k++)
				{
					satisfying[k] += mkp_problem.resource_matrix[k][j];
				}
			}
		}
	}
	free(satisfying);

	/* calculate profit */
	for (i = 0; i < mkp_problem.num_variables; i++)
	{
		profit += bitstr_test(x_solution_bits, i) * mkp_problem.profit[i];
	}

	return profit;
}

