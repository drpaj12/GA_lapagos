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
#include <sstream>

#include "types.h"
#include "globals.h"
#include "utils.h"
#include "GA_lapagos.h"

#include "argparse.hpp"
#include "read_xml_config_file.h"

#include "genetic_algorithm.h"

#include "generate_hw.h"

#include "thread_utils.h"

#include "tsp_ga.h"
#include "tsp_ga_genome_combinatorial_tupple.h"
#include "tsp_ga_threaded.h"
#include "tsp_ga_genome_combinatorial_tupple_threaded.h"
#include "tsp_adjacency.h"
#include "tsp_cartesian.h"
#include "tsp_hw_generator.h"

#include "mkp_ga.h"
#include "mkp_ga_genome_binary.h"
#include "mkp_dat.h"

#include "psns_ga.h"
#include "psns_ga_genome_binary.h"
#include "psns_dat.h"

/* globals */
global_args_t global_args;
ga_pop_t genomes;
FILE *fresult_out;
FILE *ftest_out;
FILE *fhardware_out;
double global_best;

/* Hardware Variables */
ga_hw_t ga_hw_data;

const char *ga_types_name[] = { 
                                "TSP_GA", 
                                "CONWAY_GA", 
                                "PSNS_GA",
                                "MKP_GA"
                                };
int main(int argc, char **argv)
{
	int final_pop_idx; 
    int i;

	printf("--------------------------------------------------------------------\n");
	printf("Welcome the GA-lapagos a framework for experimenting with GAs\n");
	printf("Email: jamieson.peter@gmail.com\n\n");

	/* get the command line options */
	get_options(argc, argv);

    /* read config file */
    read_config_file(global_args.config_file);

    /* check parameters */
    if (genomes.num_population < 100)
    {
        printf("Warning: Less than 100 in population can cause index miscalculations and potential bugs\n");
    }
    
    /* open final ouput files */
    fresult_out = fopen(global_args.output_file, "w");
    oassert(fresult_out != NULL);
    ftest_out = fopen(global_args.test_file, "w");
    oassert(ftest_out != NULL);
    if (global_args.mode == GENERATE_HW)
    {
        fhardware_out = fopen(global_args.hardware_file, "w");
        oassert(fhardware_out != NULL);
    }
    
    /* This is used in the exit condition...just have to set it to something */
    global_best = 0;

	/* set randomization */
    srand(configuration.rand_seed);
    rand_float_seed(configuration.rand_seed);
	printf("rand seed: %d\n", configuration.rand_seed);
	
    /* ---- BASIC Sequential GA Executions ---- */
	if (configuration.ga_type == TSP_GA && global_args.mode == GA_C)
	{
	    /* read or create the proper problem */
	    /* initialize the population parameters */
		tsp_setup_problem();
	
        /* create the double population to make easy copy to and from */
	    ga_init(tsp_create_population);
	
        /* call the run of the ga */
        tsp_run_ga();
	
        /* clean the data structures up */
        ga_clean(tsp_free_population);
        tsp_free_problem();
	}
	else if (configuration.ga_type == MKP_GA)
	{
        /* read or create the proper problem */
	    /* initialize the population parameters */
		mkp_setup_problem();
	
        /* create the double population to make easy copy to and from */
	    ga_init(mkp_create_population);
	
        /* call the run of the ga */
        mkp_run_ga();
	
        /* clean the data structures up */
        ga_clean(mkp_free_population);
        mkp_free_problem();
	}
	else if (configuration.ga_type == PSNS_GA)
	{
        /* read or create the proper problem */
	    /* initialize the population parameters */
		psns_setup_problem();
	
        /* create the double population to make easy copy to and from */
        psns_do_in_ga_init();
	
        /* call the run of the ga */
        psns_run_ga();
	
        /* clean the data structures up */
        psns_do_in_ga_clean();
	
        psns_free_problem();
	}
    /* ---- THREADED Sequential Execution - Special Mode for Parallel with pthreads ---- */
    else if (configuration.ga_type == TSP_GA && global_args.mode == GA_THREADED)
    {
        pthread_t *thread_id;
        thread_t *ts;
        pthread_mutex_t lock;
        pthread_cond_t cv;
        int ndone = 0;

        pthread_mutex_init(&lock, NULL);
        pthread_cond_init(&cv, NULL);

        thread_id = (pthread_t*)malloc(sizeof(pthread_t)*configuration.num_threads);
        ts = (thread_t*)malloc(sizeof(thread_t) * configuration.num_threads);

        /* read or create the proper problem */
	    /* initialize the population parameters */
		tsp_setup_problem();
	
        /* create the double population to make easy copy to and from */
	    ga_init(tsp_create_population);
	
        for (i = 0; i < configuration.num_threads; i++)
        {
            ts[i].lock = &lock;
            ts[i].cv = &cv;
            ts[i].ndone = &ndone;
            ts[i].id = i;

            pthread_create(&thread_id[i], NULL, tsp_run_ga_threaded, (void*)&ts[i]);
        }

        /* waiting for all threads */
        for (i = 0; i < configuration.num_threads; i++)
        {
            pthread_join(thread_id[i], NULL);
        }

        /* clean up the ids and thread structures */
        free(thread_id);
        free(ts);

        /* clean the data structures up */
        ga_clean(tsp_free_population);
        tsp_free_problem();
    }
    /* ---- HARDWARE GENERATOR - Special Mode for outputting variables that can be used in HLS flow ---- */
	else if (configuration.ga_type == TSP_GA && global_args.mode == GENERATE_HW)
	{
        /* read or create the proper problem */
	    /* initialize the population parameters */
		tsp_setup_problem();
	
        /* create the double population to make easy copy to and from */
	    ga_init(tsp_create_population);
	
        /* generate the base variables */
        /* generate the memory initialization for the genomes for this problem */
        generate_hw_variables(fhardware_out, 
                                        tsp_genome_size,
                                        tsp_genome_type,
                                        tsp_print_genome_array); 

        /* generate the tsp problem */
        tsp_generate_adjacency_array(fhardware_out);
	
        /* clean the data structures up */
        ga_clean(tsp_free_population);
        tsp_free_problem();
	}
	else
	{
		printf("Unsupported GA type\n");
	}
	
	/*-------------------FREE_PROBLEM------------------*/
	/* free the problem */
	fclose(fresult_out);
	fclose(ftest_out);

	return 1;
}

/*---------------------------------------------------------------------------------------------
 * (function: get_options)
 *-------------------------------------------------------------------------------------------*/
void get_options(int argc, char** argv) 
{
	auto parser = argparse::ArgumentParser(argv[0]);

	global_args.program_name = parser.prog();
	
	parser.add_argument(global_args.config_file, "-c")
			.help("Configuration file")
			.metavar("XML_CONFIGURATION_FILE")
			.default_value("no_config_file.xml")
			;
	
	parser.add_argument(global_args.output_file, "-o")
			.help("Output file path")
			.default_value("default_out.txt")
			.metavar("OUTPUT_FILE_PATH")
			;
	
	parser.add_argument(global_args.test_file, "-t")
			.help("Test Output file path")
			.default_value("default_test_out.txt")
			.metavar("TEST_FILE_PATH")
			;
	
	    parser.add_argument(global_args.hardware_file, "-g")
			.help("Hardware Generation file")
			.default_value("hardware_out.txt")
			.metavar("HARDWARE_OUTPUT_FILE")
			;
	
	    parser.add_argument(global_args.mode, "-m")
			.help("Mode to run in {GA_C = 0, GA_THREADED = 1, GENERATE_HW = 2}")
			.default_value("0")
			.metavar("MODE")
			;
	
	parser.add_argument(global_args.show_help, "-h")
			.help("Display this help message")
			.action(argparse::Action::HELP)
			;
	
	parser.parse_args(argc, argv);
}