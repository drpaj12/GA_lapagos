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

#include "tsp_globals.h"
#include "tsp_types.h"

#include "tsp_cartesian.h"

/* Prototypes */
short **tsp_init_grid_init(int x, int y);
void tsp_free_grid(int x, short ***grid);
double cartesian_distance(int x1, int y1, int x2, int y2);

/* globals */
tsp_problem_coordinates_t *tsp_problem_cartesian;

/*---------------------------------------------------------------------------------------------
 * (function: tsp_free_problem)
 *-------------------------------------------------------------------------------------------*/
void tsp_free_problem_cartesian()
{
	int i; 
	for (i = 0; i < tsp_problem.num_cities; i++)
	{
		free(tsp_problem_cartesian->cities[i]);
	}

	free(tsp_problem_cartesian->cities);
	free(tsp_problem_cartesian);
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_init_problem)
 *-------------------------------------------------------------------------------------------*/
void tsp_init_problem_cartesian(int num_cities)
{
	short **grid;
	int i;
	int grid_size = (int)(sqrt(num_cities) * 3);
		
	printf("Initializing a random problem\n");

	/* initialize the basic problem */
	tsp_problem_cartesian = (tsp_problem_coordinates_t*)malloc(sizeof(tsp_problem_coordinates_t));

	/* initalize the grid that makes sure we don't have two cities in one spot */
	grid = tsp_init_grid_init(grid_size, grid_size);

	/* up to 32767 cities */
	tsp_problem.num_cities = num_cities;
	tsp_problem_cartesian->cities = (tsp_city_t**)malloc(sizeof(tsp_city_t*) * tsp_problem.num_cities);
	for (i = 0; i < tsp_problem.num_cities; i++)
	{
		tsp_problem_cartesian->cities[i] = (tsp_city_t*)malloc(sizeof(tsp_city_t));
		tsp_problem_cartesian->cities[i]->id = i;

		tsp_problem_cartesian->cities[i]->x = rand()%grid_size;
		tsp_problem_cartesian->cities[i]->y = rand()%grid_size;
		
		/* check if there's a city already there */
		while (grid[tsp_problem_cartesian->cities[i]->x][tsp_problem_cartesian->cities[i]->y] != 0)
		{
			tsp_problem_cartesian->cities[i]->x = rand()%grid_size;
			tsp_problem_cartesian->cities[i]->y = rand()%grid_size;
		}

		grid[tsp_problem_cartesian->cities[i]->x][tsp_problem_cartesian->cities[i]->y] = 1;
	}
	
	/* free initialization memory */	
	tsp_free_grid(grid_size, &grid);
} 

/*---------------------------------------------------------------------------------------------
 * (function: tsp_init_grid_init)
 *-------------------------------------------------------------------------------------------*/
short **tsp_init_grid_init(int x, int y)
{
	int i, j;
	short **grid;

	grid = (short **)malloc(sizeof(short*)*x);

	for (i = 0; i < x; i++)
	{
		grid[i] = (short *)malloc(sizeof(short)*y);
		for (j = 0; j < y; j++)
		{
			grid[i][j] = 0;
		}
	}

	return grid;
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_free_grid)
 *-------------------------------------------------------------------------------------------*/
void tsp_free_grid(int x, short ***grid)
{
	int i; 

	for (i = 0; i < x; i++)
	{
		free(grid[0][i]);
	}
	free(grid[0]);
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_cost_function)
 *-------------------------------------------------------------------------------------------*/
double tsp_cost_function_from_cartesian(void *genome)
{
	int i;
	double cost = 0.0;
	int *solution = (int*)genome;

	for (i = 1; i < tsp_problem.num_cities; i++)
	{
		cost += cartesian_distance(tsp_problem_cartesian->cities[solution[i]]->x, tsp_problem_cartesian->cities[solution[i]]->y, tsp_problem_cartesian->cities[solution[i-1]]->x, tsp_problem_cartesian->cities[solution[i-1]]->y);
	}

	return cost;
}

/*---------------------------------------------------------------------------------------------
 * (function: cartesian_distance)
 *-------------------------------------------------------------------------------------------*/
double cartesian_distance(int x1, int y1, int x2, int y2)
{
	/* calculate distance */
	return sqrt(pow((x1-x2), 2) + pow((y1-y2), 2));
}


