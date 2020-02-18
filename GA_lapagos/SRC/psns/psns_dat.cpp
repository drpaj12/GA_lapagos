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

#include "psns_globals.h"
#include "psns_types.h"
#include "psns_dat.h"

/* Prototypes */

/* globals */
double *base_station_x_vals;
double *base_station_y_vals;
int *UE_2_base_station;

/*-------------------------------------------------------------------------
 * (function: read_psns_benchmark_dat)
 * --- format
 * nUEs // number of User Equipment
 * nTimeSteps // number of time steps
 * max_X max_Y // the max size of the space
 * {GBR list} // |{GBR list}| = nUEs ... all integers
 * {Priority List // |{Priority list}| = nUEs ... all integers
 * {Locations} // x and y per time |{Locations}| = 2*nUEs ... all integers x, y
 * " ... // as above per nTimeSteps
 *-----------------------------------------------------------------------*/
void read_psns_problem_dat(char *benchmark_name)
{
    FILE *f_benchmark;
    int i,j;

    f_benchmark = fopen(benchmark_name, "r");

    /* read num base_stations */
    fscanf(f_benchmark, "%d", &psns_problem.num_base_stations);

    /* read UEs */
    fscanf(f_benchmark, "%d", &psns_problem.num_UEs);

    /* read time steps */
    fscanf(f_benchmark, "%d", &psns_problem.num_time_steps);

    /* read map size */
    fscanf(f_benchmark, "%d", &psns_problem.map_size_x);
    fscanf(f_benchmark, "%d", &psns_problem.map_size_y);

    if ((psns_problem.problem_type == PSNS_DAT_FORMAT_BITS) || (psns_problem.problem_type == PSNS_DAT_FORMAT_GRAY_CODE))
    {
        psns_genome_bits.num_bits_x = (int)ceil(log(psns_problem.map_size_x)/log(2));
        psns_genome_bits.num_bits_y = (int)ceil(log(psns_problem.map_size_y)/log(2));
        psns_genome_bits.num_bits_xy = psns_genome_bits.num_bits_x + psns_genome_bits.num_bits_y;
        psns_genome_bits.num_bits = psns_genome_bits.num_bits_xy * psns_problem.num_base_stations; 
        //printf("%d %d\n", psns_genome_bits.num_bits_x, psns_genome_bits.num_bits_y);
        base_station_x_vals = (double*)malloc(sizeof(double)*psns_problem.num_base_stations); 
        base_station_y_vals = (double*)malloc(sizeof(double)*psns_problem.num_base_stations); 
        UE_2_base_station = (int*)malloc(sizeof(int)*psns_problem.num_UEs);
    }

    /* allocate and read guaranteed bit rate */
    psns_problem.gbr = (int*)malloc(sizeof(int)*psns_problem.num_UEs);
    for (i = 0; i < psns_problem.num_UEs; i++)
    {
        fscanf(f_benchmark, "%d", &psns_problem.gbr[i]);
    }

    /* allocate and read guaranteed bit rate */
    psns_problem.priorities = (int*)malloc(sizeof(int)*psns_problem.num_UEs);
    for (i = 0; i < psns_problem.num_UEs; i++)
    {
        fscanf(f_benchmark, "%d", &psns_problem.priorities[i]);
    }

    /* allocate and read resource matrix */
    psns_problem.locations = (xy_t**)malloc(sizeof(xy_t*)*psns_problem.num_UEs);
    for (j = 0; j < psns_problem.num_UEs; j++)
    {
        psns_problem.locations[j] = (xy_t*)malloc(sizeof(xy_t)*psns_problem.num_time_steps);
        for (i = 0; i < psns_problem.num_time_steps; i++)
        {
            fscanf(f_benchmark, "%lf", &psns_problem.locations[j][i].x); 
            fscanf(f_benchmark, "%lf", &psns_problem.locations[j][i].y); 
        }
    }
 
    fclose(f_benchmark);

	return;
}

/*-------------------------------------------------------------------------
 * (function: free_configuartion)
 *-----------------------------------------------------------------------*/
void free_psns_problem_dat()
{
    int i;

    if (psns_problem.num_UEs != 0)
    {
	    free(psns_problem.gbr);
	    free(psns_problem.priorities);
	    
	    for (i = 0; i < psns_problem.num_UEs; i++)
	    {
	        free(psns_problem.locations[i]);
	    }
	    free(psns_problem.locations);

        if (psns_problem.problem_type == PSNS_DAT_FORMAT_BITS)
        {
            free(base_station_x_vals);
            free(base_station_y_vals);
        }
    }
}

/*---------------------------------------------------------------------------------------------
 * (function: psns_cost_function_bits)
 *-------------------------------------------------------------------------------------------*/
double psns_cost_function_bits(void *genome)
{
	int i;
    bstr x_solution_bits = (bstr)genome;
    int idx_start = 0;
    int idx_end = psns_genome_bits.num_bits_x;

    /* calculate base_station locations */
    for (i = 0; i < psns_problem.num_base_stations; i++)
    {
        base_station_x_vals[i] = (double)bitstr_int(x_solution_bits, idx_start, idx_end);
        idx_start += psns_genome_bits.num_bits_x;
        idx_end += psns_genome_bits.num_bits_y;

        base_station_y_vals[i] = (double)bitstr_int(x_solution_bits, idx_start, idx_end);
        idx_start += psns_genome_bits.num_bits_y;
        idx_end += psns_genome_bits.num_bits_x;
    }

    return psns_cost_function_after_assignment();
}

/*---------------------------------------------------------------------------------------------
 * (function: psns_cost_function_after_assignment)
 *
 * Matlab code:
 function score = score_calculation(app, center_list, affi_list)
            % distance square list
1            dist_sq_list = zeros(app.UE_num, 1);
2            min_dist_square = 10^2;
3            for i = 1 : app.UE_num
4                dist_sq_list(i) = (center_list(affi_list(i)).x - app.UE_list(i).x)^2 + ...
5                    (center_list(affi_list(i)).y - app.UE_list(i).y)^2;
6                if dist_sq_list(i) < min_dist_square
7                    dist_sq_list(i) = min_dist_square;
8                end
9            end
10            % bitrate list
11            bitrate_list = zeros(app.UE_num, 1);
12            p1 = 0.5223;
13            p2 = 4.6188;
14            CONS_RADIATION = 50000;
15            num_RB = 10; % [Peter] think its number of radio bands?
16            spec_eff_723_1 = [0, 0.1523, 0.2344, 0.377, 0.6016, ...
17            0.877, 1.1758, 1.4766, 1.9141, 2.4063, 2.7305, ...
18            3.3223, 3.9023, 4.5234, 5.1152, 5.5547];
19            for i = 1 : app.UE_num
20                snr = 10 * log10(CONS_RADIATION / dist_sq_list(i));
21                CQI = floor(p1 * snr + p2);
22                if CQI > 15
23                    CQI = 15;
24                end
25                if CQI < 0
26                    CQI = 0;
27                end
28                bitrate_list(i) = spec_eff_723_1(CQI + 1) * num_RB; % bit per milisecond
29            end
30
31            score = 0;
32            % calculate score
33            for i = 1 : app.UE_num
34                if bitrate_list(i) >= app.UE_list(i).GBR
35                    score = score + app.UE_list(i).priority;
36                end
37            end
38        end
 *-------------------------------------------------------------------------------------------*/
double psns_cost_function_after_assignment()
{
	int i, j, k;
    int idx_start = 0;
    int idx_end = psns_genome_bits.num_bits_x;

    double spec_eff_723_1[16] = {0, 0.1523, 0.2344, 0.377, 0.6016, 0.877, 1.1758, 1.4766, 1.9141, 2.4063, 2.7305, 3.3223, 3.9023, 4.5234, 5.1152, 5.5547};
    double p1 = 0.5223;
    double p2 = 4.6188;
    double CONS_RADIATION = 50000; 
    int num_RB = 10;
    double snr;
    int CQI;
    double dist_square;
    double score = 0.0;

    for (i = 0; i < psns_problem.num_time_steps; i++)
    {
        /* calculate assignments based on time step */
        psns_assign_UE_to_BS(i);

        for (j = 0; j < psns_problem.num_UEs; j++)
        {
            /* base2ue is the assigned base station */
            dist_square = 
                        pow((psns_problem.locations[j][i].x - base_station_x_vals[UE_2_base_station[j]]), 2) + 
                        pow((psns_problem.locations[j][i].y - base_station_y_vals[UE_2_base_station[j]]), 2);

            if (dist_square < 100) // 10^2 in matlab line 2
                dist_square = 100;

            /* Matlab lines 11 to 37 */ 
            snr = 10 * log10(CONS_RADIATION/dist_square);
            CQI = floor(p1 * snr + p2);

            if (CQI > 15)
                CQI = 15;
            else if (CQI < 0)
                CQI = 0;

            if (spec_eff_723_1[CQI] * num_RB >= psns_problem.gbr[j])
            {
                score = score + psns_problem.priorities[j];
            }
        }
    }

	return score;
}

/*---------------------------------------------------------------------------------------------
 * (function: psns_assign_UE_to_BS)
 *-------------------------------------------------------------------------------------------*/
void psns_assign_UE_to_BS(int time_step)
{
    int j, k;
    int base2ue_cost;
    int base2ue_current;
    int new_cost;

    for (j = 0; j < psns_problem.num_UEs; j++)
    {
        base2ue_cost = abs(psns_problem.locations[j][time_step].x - base_station_x_vals[0]) + abs(psns_problem.locations[j][time_step].y - base_station_y_vals[0]);
        UE_2_base_station[j] = 0;

        /* Matlab lines 2 to 9 with allocation */
        for (k = 1; k < psns_problem.num_base_stations; k++)
        {
            new_cost = abs(psns_problem.locations[j][time_step].x - base_station_x_vals[k]) + abs(psns_problem.locations[j][time_step].y - base_station_y_vals[k]);

            if (new_cost < base2ue_cost)
            {
                base2ue_cost = new_cost;
                UE_2_base_station[j] = k;
            }
        }
    }
}

/*---------------------------------------------------------------------------------------------
 * (function: psns_cost_function_gray_code_bits)
 *-------------------------------------------------------------------------------------------*/
double psns_cost_function_gray_code_bits(void *genome)
{
	int i;
    bstr x_solution_bits = (bstr)genome;
    int idx_start = 0;
    int idx_end = psns_genome_bits.num_bits_x;

    /* calculate base_station locations */
    for (i = 0; i < psns_problem.num_base_stations; i++)
    {
        base_station_x_vals[i] = (double)bitstr_gray_int(x_solution_bits, idx_start, idx_end);
        idx_start += psns_genome_bits.num_bits_x;
        idx_end += psns_genome_bits.num_bits_y;

        base_station_y_vals[i] = (double)bitstr_gray_int(x_solution_bits, idx_start, idx_end);
        idx_start += psns_genome_bits.num_bits_y;
        idx_end += psns_genome_bits.num_bits_x;
    }

    return psns_cost_function_after_assignment();
}
