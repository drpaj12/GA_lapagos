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

#include "genetic_algorithm_selectors_hw.h"

/* Prototypes */

/* globals */

/*---------------------------------------------------------------------------------------------
 * (function: setup_selector_funcion)
 *-------------------------------------------------------------------------------------------*/
int setup_selector_function_hw()
{
    switch(ga_hw_data.selector_type)
    {
        case ROULETTE:
            return selector_roulette_hw();
        case SUS:
            printf("Not supported for hw hls\n");
            return selector_sus_hw();
        case TOURNAMENT:
            return selector_tournament_hw();
        case RANK:
            printf("Not supported for hw hls\n");
            return selector_rank_hw();
        case RANDOM:
            return selector_random_hw();
        case TRUNCATE:
            return selector_truncate_hw();
        default:
            return 0;
    }
}

/*---------------------------------------------------------------------------------------------
 * (function: setup_selector_funcion)
 *-------------------------------------------------------------------------------------------*/
void setup_selector_init_function_hw(int value)
{
    switch(ga_hw_data.selector_type)
    {
        case ROULETTE:
            selector_roulette_init_hw(value);
            break;
        case SUS:
            selector_sus_init_hw(value);
            break;
        case TOURNAMENT:
            selector_tournament_init_hw(value);
            break;
        case RANK:
            selector_rank_init_hw(value);
            break;
        case RANDOM:
            selector_random_init_hw(value);
            break;
        case TRUNCATE:
            selector_truncate_init_hw(value);
            break;
        default:
            break;
    }
}

int total_cost_int;
/*---------------------------------------------------------------------------------------------
 * (function: selector_roulette_init)
 *-------------------------------------------------------------------------------------------*/
void selector_roulette_init_hw(int dummy)
{
    int i;

    total_cost_int = 0;

    for (i = 0; i < ga_hw_data.num_population; i++)
    {
        total_cost_int += ga_hw_data.costs[i];
    }
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_roulette)
 * Holland (1975) Adaptation in natural and artificial systems: an introductory analysis with applications to biology, control, and artificial intelligen
 *-------------------------------------------------------------------------------------------*/
int selector_roulette_hw()
{
    int spin_spot;
    int incrementing_total = 0;
    int index;

    /* pick the random spot */
    spin_spot = rand() % total_cost_int;

    index = 0;
    
    while (spin_spot > incrementing_total)
    {
        incrementing_total += ga_hw_data.costs[index];
        index ++;
    }

    return index % ga_hw_data.num_population;
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_roulette_init)
 *-------------------------------------------------------------------------------------------*/
void selector_sus_init_hw(int num_selecting)
{
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_sus)
 * Stochastic Universal Sampling
 * Baker (1987) Reducing Bias and Inefficiency in the Selection Algorithm
 *-------------------------------------------------------------------------------------------*/
int selector_sus_hw()
{
    return 0;
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_tournament_init)
 *-------------------------------------------------------------------------------------------*/
void selector_tournament_init_hw(int num_selecting)
{
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_tournament)
 *-------------------------------------------------------------------------------------------*/
int selector_tournament_hw()
{
    int random;
    int leader;
    int i;

    leader = rand() % ga_hw_data.num_population;

    for (i = 0; i < (ga_hw_data.tournament_size - 1); i++)
    {
        random = rand() % ga_hw_data.num_population;

        if (ga_hw_data.costs[random] < ga_hw_data.costs[leader])
        {
            leader = random;
        }
    }

    return leader;
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_rank_init)
 *-------------------------------------------------------------------------------------------*/
void selector_rank_init_hw(int num_selecting)
{
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_rank)
 * Baker (1985) - Adaptive selection methods for genetic algorithms
 * Linear
 * Exponential
        pi = (c - 1)/(c^N - 1) * c^(N-i)
 *-------------------------------------------------------------------------------------------*/
int selector_rank_hw()
{
    return 0;
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_random_init)
 *-------------------------------------------------------------------------------------------*/
void selector_random_init_hw(int num_selecting)
{
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_random)
 *-------------------------------------------------------------------------------------------*/
int selector_random_hw()
{
    return rand() % ga_hw_data.num_population;
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_truncate)
 *-------------------------------------------------------------------------------------------*/
void selector_truncate_init_hw(int num_selecting)
{
    ga_hw_data.truncate_size = ga_hw_data.idx_end_breed_mutate_from;
}

/*---------------------------------------------------------------------------------------------
 * (function: selector_truncate)
 *  Crow & Kimura 1979, "Efficiency of truncation selection"
 *-------------------------------------------------------------------------------------------*/
int selector_truncate_hw()
{
    int random;

    random = rand() % ga_hw_data.truncate_size;

    return random;
}
