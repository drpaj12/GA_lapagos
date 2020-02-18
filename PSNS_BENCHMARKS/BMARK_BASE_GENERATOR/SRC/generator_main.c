/*

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
#include <time.h>
#include <math.h>
#include "globals.h"
#include "types.h"
#include "generator_main.h"

/* Prototypes */
void print_usage();
void get_options(int argc, char **argv);

/* globals */
global_args_t global_args;

void generate_file(FILE *f_benchmark, FILE *f_way_pts);
double normrnd(double mean, double stdDev);

int main(int argc, char **argv)
{
    FILE *f_benchmark;
    FILE *f_waypoint;

	printf("--------------------------------------------------------------------\n");
	printf("Welcome to benchmark generator for PSNs\n");
	printf("Email: jamieson.peter@gmail.com\n\n");

	/* get the command line options */
	get_options(argc, argv);

	/* open final ouput file */
	f_benchmark = fopen(global_args.output_file, "w");
	/* open final ouput file */
	f_waypoint = fopen(global_args.way_point_file, "r");

	/* set randomizer */
	srand(1);
//	srand ( time(NULL) );

    /* generate the benchmark */
    generate_file(f_benchmark, f_waypoint);

    fclose(f_benchmark);
    fclose(f_waypoint);

	return 0;
} 

/*-------------------------------------------------------------------------
 * (function: generate_file)
 * --- format
 * nUEs // number of User Equipment
 * nTimeSteps // number of time steps
 * max_X max_Y // the max size of the space
 * {GBR list} // |{GBR list}| = nUEs ... all integers
 * {Priority List // |{Priority list}| = nUEs ... all integers
 * {Locations} // x and y per time |{Locations}| = 2*nUEs ... all integers x, y
 * " ... // as above per nTimeSteps
 *-----------------------------------------------------------------------*/
void generate_file(FILE *f_benchmark, FILE *f_way_pts)
{
    int i,j;
    double GBR_mean[9] = {37, 37, 43, 42, 42, 43, 46, 51, 55};
    double GBR_sigma[9] = {3,  3,  4,  4,  4,  4,  4,  5,  5};
    int gbr_idx;
    int policy_idx;
    int POLICY_CONS[12] = {100,50,20,50,25,10,20,10,5,10,5,1};
    char line[16000];
    long pos;

    /* read num base_stations */
    fprintf(f_benchmark, "%d\n", global_args.num_base_stations);

    /* read UEs */
    fprintf(f_benchmark, "%d\n", global_args.num_UEs);

    /* read time steps */
    fprintf(f_benchmark, "%d\n", global_args.time_steps_in_waypoints);

    /* read map size */
    fprintf(f_benchmark, "%d ", global_args.map_size_x);
    fprintf(f_benchmark, "%d\n", global_args.map_size_y);

    /*
    GBR_mean = [37, 37, 43, 42, 42, 43, 46, 51, 55];
    GBR_sigma = [3,  3,  4,  4,  4,  4,  4,  5,  5];
    */
    /* allocate and read guaranteed bit rate */
    for (i = 0; i < global_args.num_UEs; i++)
    {
        gbr_idx = rand() % 9;
        fprintf(f_benchmark, "%d ", (int)floor(normrnd(GBR_mean[gbr_idx], GBR_sigma[gbr_idx]))) ;
    }
    fprintf(f_benchmark, "\n");

    /*
    POLICY_CONS = [100 50 20; ...
                50 25 10; ...
                20 10 5; ...
                10, 5, 1];
    */
    /* allocate and read guaranteed priorities */
    for (i = 0; i < global_args.num_UEs; i++)
    {
        policy_idx = rand() % 12;
        fprintf(f_benchmark, "%d ", POLICY_CONS[policy_idx]);
    }
    fprintf(f_benchmark, "\n");

    while(TRUE)
    {
        double point;
        pos = ftell(f_way_pts);
        fgets(line, 16000, f_way_pts);

        if (line[0] != '#')
        {
            fseek(f_way_pts, pos, SEEK_SET);
        
            /* allocate and read resource matrix */
            for (j = 0; j < global_args.num_UEs; j++)
            {
                for (i = 0; i < global_args.num_time_steps; i++)
                {
                    if (i >= global_args.time_steps_in_waypoints)
                    {
                        /* skip */
	                    fscanf(f_way_pts, "%lf", &point); 
	                    fscanf(f_way_pts, "%lf", &point); 
	                    fscanf(f_way_pts, "%lf", &point); 
	                    fscanf(f_way_pts, "%lf", &point); 
                    }
                    else
                    {
	                    /* node */
	                    fscanf(f_way_pts, "%lf", &point); 
	                    /* time */
	                    fscanf(f_way_pts, "%lf", &point); 
	                    /* x */
	                    fscanf(f_way_pts, "%lf", &point); 
	                    fprintf(f_benchmark, "%lf ", point);
	                    /* y */
	                    fscanf(f_way_pts, "%lf", &point); 
	                    fprintf(f_benchmark, "%lf ", point);
                    }
                }

                fprintf(f_benchmark, "\n");
            }

            break;
        }
    }

	return;
}

static const char *optString = "o:b:u:x:y:t:T:w:"; // list must end in ":"
/*---------------------------------------------------------------------------------------------
 * (function: get_options)
 *-------------------------------------------------------------------------*/
void get_options(int argc, char **argv)
{
	int opt = 0;

	/* set up the global arguments to there defualts */
	global_args.output_file = "default.out";
    global_args.num_base_stations = 4;
    global_args.num_UEs = 9;
    global_args.map_size_x = 200;
    global_args.map_size_y = 200;
    global_args.num_time_steps = 2;

	/* read in the option line */
	opt = getopt(argc, argv, optString);
	while(opt != -1) 
	{
		switch(opt)
		{
			/* output file */
			case 'o':
				global_args.output_file = optarg;
			break;
			case 'w':
				global_args.way_point_file = optarg;
			break;
			case 'b':
				global_args.num_base_stations = atoi(optarg);
			break;
			case 'u':
				global_args.num_UEs = atoi(optarg);
			break;
			case 'x':
				global_args.map_size_x = atoi(optarg);
			break;
			case 'y':
				global_args.map_size_y = atoi(optarg);
			break;
			case 't':
				global_args.num_time_steps = atoi(optarg);
			break;
			case 'T':
				global_args.time_steps_in_waypoints = atoi(optarg);
			break;
			default :
				printf("Invalid arguments.\n");
				print_usage();
			break;
		}

		opt = getopt(argc, argv, optString);
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: print_usage)
 *-------------------------------------------------------------------------*/
void print_usage()
{
	printf
	(
			"Usage: ga_test.exe\n"
			"\tOne of:\n"
				"\t\t-o <out_file_name.xml>\n"
				"\t\t-b <num_base_stations>\n"
				"\t\t-u <num_UEs>\n"
				"\t\t-x <x_size>\n"
				"\t\t-y <y_size>\n"
				"\t\t-t <num_time_steps>\n"
	);
	fflush(stdout);
}

/*
In MATLAB, function normrnd(mu,sigma) 
*/
double normrnd(double mean, double stdDev) 
{
    double u, v, s;
    do {
        u = ((double)rand()/(double)RAND_MAX) * 2.0 - 1.0;
        v = ((double)rand()/(double)RAND_MAX) * 2.0 - 1.0;
        s = u * u + v * v;
    } while (s >= 1 || s == 0);

    double mul = sqrt(-2.0 * log(s) / s);

    return mean + stdDev * u * mul;
}
