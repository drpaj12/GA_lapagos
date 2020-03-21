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

#include "genetic_algorithm_selectors.h"

/* Prototypes */

/* globals */
const char *selector_types_name[] = { 
										"ROULETTE", 
										"SUS",
										"TOURNAMENT",
										"RANK",
										"TRUNCATE",
										"RANDOM"
										};


/*---------------------------------------------------------------------------------------------
 * (function: setup_selector_funcion)
 *-------------------------------------------------------------------------------------------*/
void* setup_selector_function()
{
	switch(configuration.selector_type)
	{
		case ROULETTE:
			return (void*)selector_roulette;
		case SUS:
			return (void*)selector_sus;
		case TOURNAMENT:
			return (void*)selector_tournament;
		case RANK:
			return (void*)selector_rank;
		case RANDOM:
			return (void*)selector_random;
		case TRUNCATE:
			return (void*)selector_truncate;
		default:
			return NULL;
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: setup_selector_funcion)
 *-------------------------------------------------------------------------------------------*/
void* setup_selector_init_function()
{
	switch(configuration.selector_type)
	{
		case ROULETTE:
			return (void*)selector_roulette_init;
		case SUS:
			return (void*)selector_sus_init;
		case TOURNAMENT:
			return (void*)selector_tournament_init;
		case RANK:
			return (void*)selector_rank_init;
		case RANDOM:
			return (void*)selector_random_init;
		case TRUNCATE:
			return (void*)selector_truncate_init;
		default:
			return NULL;
	}
}

double total_cost;
/*---------------------------------------------------------------------------------------------
 * (function: selector_roulette_init)
 *-------------------------------------------------------------------------------------------*/
void selector_roulette_init(int dummy)
{
	int i;

	total_cost = 0;

	for (i = 0; i < genomes.num_population; i++)
	{
		total_cost += genomes.costs[i];
	}
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_roulette)
 * Holland (1975) Adaptation in natural and artificial systems: an introductory analysis with applications to biology, control, and artificial intelligen
 *-------------------------------------------------------------------------------------------*/
int selector_roulette()
{
	double spin_spot;
	double incrementing_total = 0.0;
	int index;

	/* pick the random spot */
	spin_spot = rand_float();

	index = 0;
	
	while (spin_spot > incrementing_total)
	{
		incrementing_total += genomes.costs[index]/total_cost;
		index ++;
	}

	return index % genomes.num_population;
}

double sus_hop_by;
double sus_spin_spot;
double sus_incrementing_total;
int sus_index;
/*---------------------------------------------------------------------------------------------
 * (function: selector_roulette_init)
 *-------------------------------------------------------------------------------------------*/
void selector_sus_init(int num_selecting)
{
	int i;

	total_cost = 0;

	for (i = 0; i < genomes.num_population; i++)
	{
		total_cost += genomes.costs[i];
	}

	sus_hop_by = 1.0/num_selecting;
	sus_spin_spot = random_float_in_range(0.0, sus_hop_by);
	sus_spin_spot -= sus_hop_by;

	sus_incrementing_total = 0.0;
	sus_index = 0;
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_sus)
 * Stochastic Universal Sampling
 * Baker (1987) Reducing Bias and Inefficiency in the Selection Algorithm
 *-------------------------------------------------------------------------------------------*/
int selector_sus()
{
	/* pick the random spot */
	sus_spin_spot += sus_hop_by;
	
	while (sus_spin_spot > sus_incrementing_total)
	{
		sus_incrementing_total += genomes.costs[sus_index]/total_cost;
		sus_index ++;
	}

	return sus_index % genomes.num_population;
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_tournament_init)
 *-------------------------------------------------------------------------------------------*/
void selector_tournament_init(int num_selecting)
{
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_tournament)
 *-------------------------------------------------------------------------------------------*/
int selector_tournament()
{
	int random;
	int leader;
	int i;

	leader = floor(random_float()*genomes.num_population);

	for (i = 0; i < (genomes.tournament_size - 1); i++)
	{
		random = floor(random_float()*genomes.num_population);

		if (genomes.costs[random] < genomes.costs[leader])
		{
			leader = random;
		}
	}

	return leader;
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_rank_init)
 *-------------------------------------------------------------------------------------------*/
void selector_rank_init(int num_selecting)
{
	/* 1+2+3+4+5+6 or using the gauss formula (6+1)*6/2 = 21.*/
	total_cost = (double)((genomes.num_population+1)*(genomes.num_population/2));
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_rank)
 * Baker (1985) - Adaptive selection methods for genetic algorithms
 * Linear
 * Exponential
		pi = (c - 1)/(c^N - 1) * c^(N-i)
 *-------------------------------------------------------------------------------------------*/
int selector_rank()
{
	double spin_spot;
	double incrementing_total = 0.0;
	int index;
	int down_index;

	/* pick the random spot */
	spin_spot = rand_float();

	index = 0;
	down_index = genomes.num_population;
	
	while (spin_spot > incrementing_total)
	{
		incrementing_total += (down_index)/total_cost;
		index ++;
		down_index --;
	}

	return index % genomes.num_population;


}

/*---------------------------------------------------------------------------------------------
 * (function: selector_random_init)
 *-------------------------------------------------------------------------------------------*/
void selector_random_init(int num_selecting)
{
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_random)
 *-------------------------------------------------------------------------------------------*/
int selector_random()
{
	int random;

	random = floor(random_float()*genomes.num_population);

	return random % genomes.num_population;
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_truncate)
 *-------------------------------------------------------------------------------------------*/
void selector_truncate_init(int num_selecting)
{
	genomes.truncate_size = (int)(floor(genomes.num_population * genomes.top_percent_to_breed_from));
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_truncate)
 *	Crow & Kimura 1979, "Efficiency of truncation selection"
 *-------------------------------------------------------------------------------------------*/
int selector_truncate()
{
	int random;

	random = floor(random_float()*genomes.truncate_size);

	return random;
}
