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
#include "psns_kmeans.h"

/* number of iterations in kmeans */
#define K_ITERATIONS 15

/* Prototypes */

/*-------------------------------------------------------------------------
 * (function: psns_solve_by_kmeans)
         function center_list = calc_cluster_centroid(app, ue_affi_list)
            center.x = 0;
            center.y = 0;
            center.num_ue = 0;
            center_list = repmat(center, 1, app.cluster_num);
            for i = 1 : app.UE_num
                cluster_id = ue_affi_list(i);
                center_list(cluster_id).num_ue = center_list(cluster_id).num_ue + 1;
                center_list(cluster_id).x = center_list(cluster_id).x + app.UE_list(i).x;
                center_list(cluster_id).y = center_list(cluster_id).y + app.UE_list(i).y;
            end
            for j = 1 : app.cluster_num
                center_list(j).x = floor(center_list(j).x / center_list(j).num_ue);
                center_list(j).y = floor(center_list(j).y / center_list(j).num_ue);
            end
        end
 *-----------------------------------------------------------------------*/
void psns_solve_by_kmeans()
{
    int i, j;
    int *num_UEs_per_base_station;
    int base_station_idx;

    num_UEs_per_base_station = (int*)calloc(psns_problem.num_base_stations, sizeof(int));

    /* initialize the points equally */
    psns_initialize_clusters(psns_problem.map_size_x, psns_problem.map_size_y, psns_problem.num_base_stations);

    for (i = 0; i < K_ITERATIONS; i++)
    {
        psns_assign_UE_to_BS(0);

        for(j = 0; j < psns_problem.num_UEs; j++)
        {
            base_station_idx = UE_2_base_station[j];
            num_UEs_per_base_station[base_station_idx] ++;

            base_station_x_vals[base_station_idx] += psns_problem.locations[j][0].x;
            base_station_y_vals[base_station_idx] += psns_problem.locations[j][0].y;
        }

        for(j = 0; j < psns_problem.num_base_stations; j++)
        {
            /* each should have a min of 1 */
            num_UEs_per_base_station[j] ++;

            base_station_x_vals[j] = (base_station_x_vals[j] / num_UEs_per_base_station[j]);
            base_station_y_vals[j] = (base_station_y_vals[j] / num_UEs_per_base_station[j]);

            num_UEs_per_base_station[j] = 0;
        }
    }

    free(num_UEs_per_base_station);
}

/*-------------------------------------------------------------------------
 * (function: psns_solve_by_kmeans)
        function center_list = calc_cluster_weighted_centroid(app, ue_affi_list)
            ALPHA = 1;
            BETA = 1;
            center.x = 0;
            center.y = 0;
            center.num_p = 0;
            center_list = repmat(center, 1, app.cluster_num);
            for i = 1 : app.UE_num
                cluster_id = ue_affi_list(i);
                center_list(cluster_id).num_p = center_list(cluster_id).num_p ...
                    + (app.UE_list(i).priority^ALPHA) * (app.UE_list(i).GBR^BETA);
                center_list(cluster_id).x = center_list(cluster_id).x + ...
                    app.UE_list(i).x * (app.UE_list(i).priority^ALPHA) * (app.UE_list(i).GBR^BETA);
                center_list(cluster_id).y = center_list(cluster_id).y + ...
                    app.UE_list(i).y * (app.UE_list(i).priority^ALPHA) * (app.UE_list(i).GBR^BETA);
            end
            for j = 1 : app.cluster_num
                center_list(j).x = floor(center_list(j).x / center_list(j).num_p);
                center_list(j).y = floor(center_list(j).y / center_list(j).num_p);
            end
        end
 *-----------------------------------------------------------------------*/
void psns_solve_by_weighted_kmeans()
{
    int i, j;
    int *num_UEs_per_base_station;
    int base_station_idx;
    double ALPHA = 1.0; 
    double BETA = 1.0; 

    num_UEs_per_base_station = (int*)calloc(psns_problem.num_base_stations, sizeof(int));

    /* initialize the points equally */
    psns_initialize_clusters(psns_problem.map_size_x, psns_problem.map_size_y, psns_problem.num_base_stations);

    for (i = 0; i < K_ITERATIONS; i++)
    {
        psns_assign_UE_to_BS(0);

        for(j = 0; j < psns_problem.num_UEs; j++)
        {
            base_station_idx = UE_2_base_station[j];
            num_UEs_per_base_station[base_station_idx] += pow(psns_problem.priorities[j], ALPHA) *
                                                          pow(psns_problem.gbr[j], BETA);

            base_station_x_vals[base_station_idx] += psns_problem.locations[j][0].x * 
                                                        pow(psns_problem.priorities[j], ALPHA) *
                                                        pow(psns_problem.gbr[j], BETA);
            base_station_y_vals[base_station_idx] += psns_problem.locations[j][0].y * 
                                                        pow(psns_problem.priorities[j], ALPHA) *
                                                        pow(psns_problem.gbr[j], BETA);
        }

        for(j = 0; j < psns_problem.num_base_stations; j++)
        {
            /* each should have a min of 1 */
            num_UEs_per_base_station[j] ++;

            base_station_x_vals[j] = (base_station_x_vals[j] / num_UEs_per_base_station[j]);
            base_station_y_vals[j] = (base_station_y_vals[j] / num_UEs_per_base_station[j]);

            num_UEs_per_base_station[j] = 0;
        }
    }

    free(num_UEs_per_base_station);
}

/*-------------------------------------------------------------------------
 * (function: psns_solve_by_kmeans_with_time)
 *-----------------------------------------------------------------------*/
void psns_solve_by_kmeans_with_time()
{
    int i, j, k;
    int *num_UEs_per_base_station;
    int base_station_idx;

    num_UEs_per_base_station = (int*)calloc(psns_problem.num_base_stations, sizeof(int));

    /* initialize the points equally */
    psns_initialize_clusters(psns_problem.map_size_x, psns_problem.map_size_y, psns_problem.num_base_stations);

    for (i = 0; i < K_ITERATIONS; i++)
    {
        for (k = 0; k < psns_problem.num_time_steps; k++)
        {
	        psns_assign_UE_to_BS(k);
	
	        for(j = 0; j < psns_problem.num_UEs; j++)
	        {
	            base_station_idx = UE_2_base_station[j];
	            num_UEs_per_base_station[base_station_idx] ++;
	
	            base_station_x_vals[base_station_idx] += psns_problem.locations[j][k].x;
	            base_station_y_vals[base_station_idx] += psns_problem.locations[j][k].y;
	        }
	
	        for(j = 0; j < psns_problem.num_base_stations; j++)
	        {
	            /* each should have a min of 1 */
	            num_UEs_per_base_station[j] ++;
	
	            base_station_x_vals[j] = (base_station_x_vals[j] / num_UEs_per_base_station[j]);
	            base_station_y_vals[j] = (base_station_y_vals[j] / num_UEs_per_base_station[j]);
	
	            num_UEs_per_base_station[j] = 0;
	        }
        }
    }

    free(num_UEs_per_base_station);
}

/*-------------------------------------------------------------------------
 * (function: psns_solve_by_kmeans_with_time)
 *-----------------------------------------------------------------------*/
void psns_solve_by_weighted_kmeans_with_time()
{
    int i, j, k;
    int *num_UEs_per_base_station;
    int base_station_idx;
    double ALPHA = 1.0; 
    double BETA = 1.0; 

    num_UEs_per_base_station = (int*)calloc(psns_problem.num_base_stations, sizeof(int));

    /* initialize the points equally */
    psns_initialize_clusters(psns_problem.map_size_x, psns_problem.map_size_y, psns_problem.num_base_stations);

    for (i = 0; i < K_ITERATIONS; i++)
    {
        for (k = 0; k < psns_problem.num_time_steps; k++)
        {
	        psns_assign_UE_to_BS(k);
	
	        for(j = 0; j < psns_problem.num_UEs; j++)
	        {
	            base_station_idx = UE_2_base_station[j];
	            num_UEs_per_base_station[base_station_idx] += pow(psns_problem.priorities[j], ALPHA) *
	                                                          pow(psns_problem.gbr[j], BETA);
	
	            base_station_x_vals[base_station_idx] += psns_problem.locations[j][k].x * 
	                                                        pow(psns_problem.priorities[j], ALPHA) *
	                                                        pow(psns_problem.gbr[j], BETA);
	            base_station_y_vals[base_station_idx] += psns_problem.locations[j][k].y * 
	                                                        pow(psns_problem.priorities[j], ALPHA) *
	                                                        pow(psns_problem.gbr[j], BETA);
	        }
	
	        for(j = 0; j < psns_problem.num_base_stations; j++)
	        {
	            /* each should have a min of 1 */
	            num_UEs_per_base_station[j] ++;
	
	            base_station_x_vals[j] = (base_station_x_vals[j] / num_UEs_per_base_station[j]);
	            base_station_y_vals[j] = (base_station_y_vals[j] / num_UEs_per_base_station[j]);
	
	            num_UEs_per_base_station[j] = 0;
	        }
        }
    }

    free(num_UEs_per_base_station);
}

/*-------------------------------------------------------------------------
 * (function: psns_initialize_clusters)
 * modified from: https://stackoverflow.com/questions/6190019/split-a-rectangle-into-equal-sized-rectangles
 *-----------------------------------------------------------------------*/
void psns_initialize_clusters(int x, int y, int num_points)
{
    int columns = ceil(sqrt(num_points));
    int fullRows = num_points / columns;
    int orphans = num_points % columns;   // how many 'odd-sized' ones on our bottom row.
    int idx = 0;

    int width =  x / columns;
    int height = y / (orphans == 0 ? fullRows : (fullRows+1)); // reduce height if there are orphans

    /* Calculate starting points */
    for (int y = 0; y < fullRows; ++y)
    {
        for (int x = 0; x < columns; ++x)
        {
            base_station_x_vals[idx] = (x+1) * width - width / 2;
            base_station_y_vals[idx] = (y+1) * height - height / 2;

            idx ++;
        }
    }

    if (orphans > 0)
    {
        int orphanWidth = x / orphans;
        for (int x = 0; y < orphans; ++x)
        {
            base_station_x_vals[idx] = (x+1) * orphanWidth - orphanWidth / 2;
            base_station_y_vals[idx] = (y+1) * height - height / 2;

            idx ++;
        }
    }
}
