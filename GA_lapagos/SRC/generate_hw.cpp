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

#include "tsp_hw_generator.h"

/* Prototypes */

/* globals */

/*---------------------------------------------------------------------------------------------
 * (function: generate_hw_variables)
 *-------------------------------------------------------------------------------------------*/
void generate_hw_variables(FILE *hw_output,
                            int (*fptr_genome_size)(),
                            void (*fptr_genome_type)(FILE *),
                            void (*fptr_genome)(FILE *, population_t **))
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


    fprintf(hw_output, "#define HW_COMPILE /* top types.h  */\n");
    fprintf(hw_output, "#define GENOME_SIZE_HW %d /* types.h */\n", fptr_genome_size());
    fprintf(hw_output, "#define POPULATION_SIZE_HW %d /* types.h */\n", genomes.num_population);

    fptr_genome_type(hw_output);
    fprintf(hw_output, "population[2][POPULATION_SIZE_HW][GENOME_SIZE_HW]; /* types.h */\n");

    fprintf(hw_output, "ga_hw_t ga_hw_data = { /* data set */\n");
    fprintf(hw_output, "%d, /* ga_type*/\n", configuration.ga_type);
    fprintf(hw_output, "%d, /* crossover_type*/\n", configuration.crossover_type);
    fprintf(hw_output, "%d, /* selector_type*/\n", configuration.selector_type);
    fprintf(hw_output, "%d, /* rand seed */\n", configuration.rand_seed);
    fprintf(hw_output, "%d, /* num_population */\n", genomes.num_population);

    fprintf(hw_output, "{ /* start genome */ \n");
    fprintf(hw_output, "{ /* genome [0] */ \n");
    fptr_genome(hw_output, genomes.population[0]);
    fprintf(hw_output, "}, /* end genome [0] */\n");
    fprintf(hw_output, "{ /* genome [1] */ \n");
    fptr_genome(hw_output, genomes.population[1]);
    fprintf(hw_output, "} /* end genome [1] */\n");
    fprintf(hw_output, "}, /* end genome */\n");

    fprintf(hw_output, "{ /* costs */ ");
    for (i = 0; i < genomes.num_population; i++)
    {
        if (i == genomes.num_population - 1)
            fprintf(hw_output, "0");
        else
            fprintf(hw_output, "0, ");
    }
    fprintf(hw_output, "}, /* end costs */\n");

    fprintf(hw_output, "%d, /* idx_start_keep */\n", 0);
    fprintf(hw_output, "%d, /* idx_end_keep */\n", idx_end_keep);
    fprintf(hw_output, "%d, /* idx_start_mutate */\n", idx_start_mutate); 
    fprintf(hw_output, "%d, /* idx_end_mutate */\n", idx_end_mutate); 
    fprintf(hw_output, "%d, /* idx_start_breed */\n", idx_start_breed); 
    fprintf(hw_output, "%d, /* idx_end_breed */\n", idx_end_breed); 
    fprintf(hw_output, "%d, /* idx_start_breed_and_mutate */\n", idx_start_breed_and_mutate); 
    fprintf(hw_output, "%d, /* idx_end_breed_and_mutate */\n", idx_end_breed_and_mutate); 
    fprintf(hw_output, "%d, /* idx_start_random */\n", idx_start_random); 
    fprintf(hw_output, "%d, /* idx_end_random */\n", idx_end_random); 

    fprintf(hw_output, "%d, /* idx_end_breed_from */\n", idx_end_breed_from); 
    fprintf(hw_output, "%d, /* idx_end_mutate_from */\n", idx_end_mutate_from); 
    fprintf(hw_output, "%d, /* idx_end_breed_mutate_from  */\n", idx_end_breed_mutate_from ); 

    fprintf(hw_output, "%d, /* num_mutations  */\n", (int)floor(genomes.percent_of_genome_mutations * tsp_genome_size()));

    fprintf(hw_output, "%d, /* tournament_size  */\n", genomes.tournament_size);
    fprintf(hw_output, "%d, /* truncate_size  */\n", genomes.truncate_size);
    fprintf(hw_output, "%d, /* max_generations  */\n", genomes.max_generations);
    fprintf(hw_output, "%d /* num_parents  */\n", genomes.num_parents);

    fprintf(hw_output, "};\n");
}

