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

#include "tsp_static_memory_generator.h"

/* Prototypes */

/*---------------------------------------------------------------------------------------------
 * (function: generate_hw_variables)
 *-------------------------------------------------------------------------------------------*/
void tsp_generate_hw_variables(short mode,
							FILE *file_output,
							int (*fptr_genome_size)(),
							void (*fptr_genome_type)(FILE *),
							void (*fptr_genome2d)(FILE *, population_t **),
							void (*fptr_genome1d)(FILE *, population_t **))
{
	int i;

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


	if (mode == GENERATE_FPGA)
	{
		fprintf(file_output, "#define GENOME_SIZE_HW %d /* types.h */\n", fptr_genome_size());
		fprintf(file_output, "#define POPULATION_SIZE_HW %d /* types.h */\n", genomes.num_population);
	
		fptr_genome_type(file_output);
		fprintf(file_output, "population[2][POPULATION_SIZE_HW][GENOME_SIZE_HW]; /* types.h */\n");
	
		fprintf(file_output, "ga_hw_t ga_hw_data = { /* data set */\n");
	}
	else if (mode == GENERATE_GPU)
	{
		fprintf(file_output, "#define GENOME_SIZE_GPU %d /* types.h */\n", fptr_genome_size());
		fprintf(file_output, "#define POPULATION_SIZE_GPU %d /* types.h */\n", genomes.num_population);
	
		fptr_genome_type(file_output);
		fprintf(file_output, "population[2][POPULATION_SIZE_HW*GENOME_SIZE_HW]; /* types.h */\n");
	
		fprintf(file_output, "ga_gpu_t ga_gpu_data = { /* data set */\n");
	}
	fprintf(file_output, "%d, /* ga_type*/\n", configuration.ga_type);
	fprintf(file_output, "%d, /* crossover_type*/\n", configuration.crossover_type);
	fprintf(file_output, "%d, /* selector_type*/\n", configuration.selector_type);
	fprintf(file_output, "%d, /* rand seed */\n", configuration.rand_seed);
	fprintf(file_output, "%d, /* num_population */\n", genomes.num_population);

	fprintf(file_output, "{ /* start genomes */ \n");
	fprintf(file_output, "{ /* population [0] */ \n");
	if (mode == GENERATE_FPGA)
	{
		fptr_genome2d(file_output, genomes.population[0]);
	}
	else if (mode == GENERATE_GPU)
	{
		fptr_genome1d(file_output, genomes.population[0]);
	}
	fprintf(file_output, "}, /* end population [0] */\n");
	fprintf(file_output, "{ /* population [1] */ \n");
	if (mode == GENERATE_FPGA)
	{
		fptr_genome2d(file_output, genomes.population[1]);
	}
	else if (mode == GENERATE_GPU)
	{
		fptr_genome1d(file_output, genomes.population[1]);
	}
	fprintf(file_output, "} /* end population [1] */\n");
	fprintf(file_output, "}, /* end genomes */\n");

	fprintf(file_output, "{ /* costs */ ");
	for (i = 0; i < genomes.num_population; i++)
	{
		if (i == genomes.num_population - 1)
			fprintf(file_output, "0");
		else
			fprintf(file_output, "0, ");
	}
	fprintf(file_output, "}, /* end costs */\n");

	fprintf(file_output, "%d, /* idx_start_keep */\n", 0);
	fprintf(file_output, "%d, /* idx_end_keep */\n", idx_end_keep);
	fprintf(file_output, "%d, /* idx_start_mutate */\n", idx_start_mutate); 
	fprintf(file_output, "%d, /* idx_end_mutate */\n", idx_end_mutate); 
	fprintf(file_output, "%d, /* idx_start_breed */\n", idx_start_breed); 
	fprintf(file_output, "%d, /* idx_end_breed */\n", idx_end_breed); 
	fprintf(file_output, "%d, /* idx_start_breed_and_mutate */\n", idx_start_breed_and_mutate); 
	fprintf(file_output, "%d, /* idx_end_breed_and_mutate */\n", idx_end_breed_and_mutate); 
	fprintf(file_output, "%d, /* idx_start_random */\n", idx_start_random); 
	fprintf(file_output, "%d, /* idx_end_random */\n", idx_end_random); 

	fprintf(file_output, "%d, /* idx_end_breed_from */\n", idx_end_breed_from); 
	fprintf(file_output, "%d, /* idx_end_mutate_from */\n", idx_end_mutate_from); 
	fprintf(file_output, "%d, /* idx_end_breed_mutate_from	*/\n", idx_end_breed_mutate_from ); 

	fprintf(file_output, "%d, /* num_mutations	*/\n", (int)floor(genomes.percent_of_genome_mutations * tsp_genome_size()));

	fprintf(file_output, "%d, /* tournament_size  */\n", genomes.tournament_size);
	fprintf(file_output, "%d, /* truncate_size	*/\n", genomes.truncate_size);
	fprintf(file_output, "%d, /* max_generations  */\n", genomes.max_generations);
	fprintf(file_output, "%d /* num_parents	 */\n", genomes.num_parents);

	fprintf(file_output, "};\n");
}

#define NUM_ENTRIES_PER_LINE 30
/*-------------------------------------------------------------------------
 * (function: tsp_generate_problem_array)
 *-----------------------------------------------------------------------*/
void tsp_generate_adjacency_array_2d(FILE *file_output)
{
	int i, j;

	fprintf(file_output, "/* tsp adjacency matrix */\n");
	fprintf(file_output, "#define ADJACENCY_HW_SIZE %d /* tsp_types.h */\n", tsp_problem.num_cities);
	fprintf(file_output, "tsp_problem_adjacency_hw_t adjacency = {/* main.c */\n");

	fprintf(file_output, "{\n");
	for (i = 0; i < tsp_problem.num_cities; i++)
	{
		fprintf(file_output, "\t{");
		for (j = 0; j < tsp_problem.num_cities; j++)
		{
			if (j == (tsp_problem.num_cities - 1))
			{
				fprintf(file_output, "%d", (int)floor(tsp_problem_adjacency->adjacency_matrix[i][j]));
			}
			else if (j % NUM_ENTRIES_PER_LINE == 0 && j != 0)
			{
				fprintf(file_output, "\n\t%d, ", (int)floor(tsp_problem_adjacency->adjacency_matrix[i][j]));
			}
			else
			{
				fprintf(file_output, "%d, ", (int)floor(tsp_problem_adjacency->adjacency_matrix[i][j]));
			}
		}

		if (i == (tsp_problem.num_cities - 1))
		{
			fprintf(file_output, "}\n");
		}
		else
		{
			fprintf(file_output, "},\n");
		}
	}
	fprintf(file_output, "}\n};\n");
}

/*-------------------------------------------------------------------------
 * (function: tsp_generate_problem_array)
 *-----------------------------------------------------------------------*/
void tsp_generate_adjacency_array_1d(FILE *file_output)
{
	int i, j;

	fprintf(file_output, "/* tsp adjacency matrix */\n");
	fprintf(file_output, "#define ADJACENCY_HW_SIZE %d /* tsp_types.h */\n", tsp_problem.num_cities);
	fprintf(file_output, "tsp_problem_adjacency_gpu_t adjacency = {/* main.c */\n");

	fprintf(file_output, "{\n");
	for (i = 0; i < tsp_problem.num_cities; i++)
	{
		fprintf(file_output, "\t /* [%d] */", i);
		for (j = 0; j < tsp_problem.num_cities; j++)
		{
			if (j == (tsp_problem.num_cities - 1))
			{
				fprintf(file_output, "%d", (int)floor(tsp_problem_adjacency->adjacency_matrix[i][j]));
			}
			else if (j % NUM_ENTRIES_PER_LINE == 0 && j != 0)
			{
				fprintf(file_output, "\n\t%d, ", (int)floor(tsp_problem_adjacency->adjacency_matrix[i][j]));
			}
			else
			{
				fprintf(file_output, "%d, ", (int)floor(tsp_problem_adjacency->adjacency_matrix[i][j]));
			}
		}

		if (i == (tsp_problem.num_cities - 1))
		{
			fprintf(file_output, "}\n");
		}
		else
		{
			fprintf(file_output, ",\n");
		}
	}
	fprintf(file_output, "}\n};\n");
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
void tsp_genome_type(FILE *file_output)
{
	fprintf(file_output, "int ");
}

/*-------------------------------------------------------------------------
 * (function: tsp_print_genome_array)
 * just the internal { } for each population followed by ","
 *-----------------------------------------------------------------------*/
void tsp_print_genome_array_2d(FILE *file_output, population_t **population)
{
	int i, j;
	int *genome_int;

	for (j = 0; j < genomes.num_population; j++)
	{
		genome_int = (int*)population[j]->genome;
		fprintf(file_output, "\t{");
		for (i = 0; i < tsp_problem.num_cities; i++)
		{
			if (i == (tsp_problem.num_cities - 1))
			{
				fprintf(file_output, "%d", genome_int[i]);
			}
			else if (i % NUM_ENTRIES_PER_LINE == 0 && i != 0)
			{
				fprintf(file_output, "\n\t%d, ", genome_int[i]);
			}
			else
			{
				fprintf(file_output, "%d, ", genome_int[i]);
			}
		}

		if (j == (tsp_problem.num_cities - 1))
		{
			fprintf(file_output, "}\n");
		}
		else
		{
			fprintf(file_output, "},\n");
		}
	}
}

/*-------------------------------------------------------------------------
 * (function: tsp_print_genome_array)
 * just the internal { } for each population followed by ","
 *-----------------------------------------------------------------------*/
void tsp_print_genome_array_1d(FILE *file_output, population_t **population)
{
	int i, j;
	int *genome_int;

	for (j = 0; j < genomes.num_population; j++)
	{
		genome_int = (int*)population[j]->genome;
		fprintf(file_output, "\t /* [%d] */", j);
		for (i = 0; i < tsp_problem.num_cities; i++)
		{
			if (i == (tsp_problem.num_cities - 1))
			{
				fprintf(file_output, "%d", genome_int[i]);
			}
			else if (i % NUM_ENTRIES_PER_LINE == 0 && i != 0)
			{
				fprintf(file_output, "\n\t%d, ", genome_int[i]);
			}
			else
			{
				fprintf(file_output, "%d, ", genome_int[i]);
			}
		}

		if (j == (tsp_problem.num_cities - 1))
		{
			fprintf(file_output, "}\n");
		}
		else
		{
			fprintf(file_output, ",\n");
		}
	}
}
