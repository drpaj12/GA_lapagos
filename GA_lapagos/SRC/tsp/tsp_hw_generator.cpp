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

#include "tsp_globals.h"
#include "tsp_types.h"


/* Prototypes */

#define NUM_ENTRIES_PER_LINE 30
/*-------------------------------------------------------------------------
 * (function: tsp_generate_problem_array)
 *-----------------------------------------------------------------------*/
void tsp_generate_adjacency_array(FILE *hw_output)
{
    int i, j;

    fprintf(hw_output, "/* tsp adjacency matrix */\n");
    fprintf(hw_output, "#define ADJACENCY_HW_SIZE %d /* tsp_types.h */\n", tsp_problem.num_cities);
    fprintf(hw_output, "tsp_problem_adjacency_hw_t adjacency = {/* main.c */\n");

    fprintf(hw_output, "{\n");
    for (i = 0; i < tsp_problem.num_cities; i++)
    {
        fprintf(hw_output, "\t{");
	    for (j = 0; j < tsp_problem.num_cities; j++)
        {
            if (j == (tsp_problem.num_cities - 1))
            {
		        fprintf(hw_output, "%d", (int)floor(tsp_problem_adjacency->adjacency_matrix[i][j]));
            }
            else if (j % NUM_ENTRIES_PER_LINE == 0 && j != 0)
            {
		        fprintf(hw_output, "\n\t%d, ", (int)floor(tsp_problem_adjacency->adjacency_matrix[i][j]));
            }
            else
            {
		        fprintf(hw_output, "%d, ", (int)floor(tsp_problem_adjacency->adjacency_matrix[i][j]));
            }
        }

        if (i == (tsp_problem.num_cities - 1))
        {
            fprintf(hw_output, "}\n");
        }
        else
        {
            fprintf(hw_output, "},\n");
        }
    }
    fprintf(hw_output, "}\n};\n");
}

/*-------------------------------------------------------------------------
 * (function: tsp_genome_size)
 *-----------------------------------------------------------------------*/
int tsp_genome_size()
{
    return tsp_problem.num_cities;
}

/*-------------------------------------------------------------------------
 * (function: tsp_genome_type)
 *-----------------------------------------------------------------------*/
void tsp_genome_type(FILE *hw_output)
{
    fprintf(hw_output, "int ");
}

/*-------------------------------------------------------------------------
 * (function: tsp_print_genome_array)
 * just the internal { } for each population followed by ","
 *-----------------------------------------------------------------------*/
void tsp_print_genome_array(FILE *hw_output, population_t **population)
{
    int i, j;
    int *genome_int;

    for (j = 0; j < genomes.num_population; j++)
    {
        genome_int = (int*)population[j]->genome;
	    fprintf(hw_output, "\t{");
		for (i = 0; i < tsp_problem.num_cities; i++)
	    {
	        if (i == (tsp_problem.num_cities - 1))
	        {
	            fprintf(hw_output, "%d", genome_int[i]);
	        }
	        else if (i % NUM_ENTRIES_PER_LINE == 0 && i != 0)
	        {
	            fprintf(hw_output, "\n\t%d, ", genome_int[i]);
            }
	        else
	        {
	            fprintf(hw_output, "%d, ", genome_int[i]);
	        }
	    }

        if (j == (tsp_problem.num_cities - 1))
        {
            fprintf(hw_output, "}\n");
        }
        else
        {
            fprintf(hw_output, "},\n");
        }
    }
}
