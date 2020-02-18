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

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#include <stdlib.h>

#ifndef TYPES_H
#define TYPES_H

typedef struct global_args_t_t global_args_t;

//#define oassert(x) {if(!(x)){exit(-1);}} // causes an interrupt in GDB
#define oassert(x) {if(!(x)){__asm("int3");}} // causes an interrupt in GDB

/* the global arguments of the software */
struct global_args_t_t
{
	int i;
};

/* for the supported GA solution types */
#define NUM_GA_TYPES 4
enum GA_TYPE { 
                TSP_GA, 
                CONWAY_GA,
                PSNS_GA,
                MKP_GA
                };

/* for the modes we can run*/
enum MODES { 
                GA_C, 
                GA_FPGA,
                GENERATE_HW
                };


#define NUM_CROSSOVER_TYPES 8
enum CROSSOVER_TYPE {
                        PMX, 
                        POSITION,
                        CX,
                        CX2,
                        OX,
                        SINGLE_POINT,
                        TWO_POINT,
                        UNIFORM
                    };

#define NUM_SELECTOR_TYPES 6
enum SELECTOR_TYPE {
                        ROULETTE, 
                        SUS,
                        TOURNAMENT,
                        RANK,
                        TRUNCATE,
                        RANDOM
                    };


/* the configuration file */
typedef struct config_t_t config_t;
struct config_t_t 
{
	enum GA_TYPE ga_type;
	enum CROSSOVER_TYPE crossover_type;
	enum SELECTOR_TYPE selector_type;

	char *benchmark_file_name;
    char *problem_type;

	int rand_seed;
};

typedef struct ga_pop_t_t ga_pop_t;
typedef struct population_t_t population_t;
typedef struct ga_problem_t_t ga_problem_t;
typedef struct parent_history_t_t parent_history_t;

struct ga_pop_t_t
{
	int num_population;
	population_t ***population;

	double *costs;

	/* total for 5 params = 1 */
	float percent_to_keep;
	float percent_from_mutate;
	float percent_from_breed;
	float percent_from_breed_and_mutate;
	float percent_to_random;
	
	float top_percent_to_breed_from;
	float top_percent_to_mutate_from;
	float top_percent_to_breed_mutate_from;

	float percent_of_genome_mutations;

    int tournament_size;
    int truncate_size;
	
	int max_generations;
	int num_parents;
};

struct population_t_t
{
	long long int id;
	void *genome;
	parent_history_t *parents;
};

struct parent_history_t_t
{
	long long int *ids;
};

typedef struct str_t {
    int len, alloc;
    unsigned char *s;
} bstr_t, *bstr;
#define str_len(s) ((s)->len)
 
#define GENOME_SIZE_HW 198 /* types.h */
#define POPULATION_SIZE_HW 40 /* types.h */
typedef struct ga_hw_t_t ga_hw_t;
struct ga_hw_t_t
{
	enum GA_TYPE ga_type;
	enum CROSSOVER_TYPE crossover_type;
	enum SELECTOR_TYPE selector_type;

	int rand_seed;

	int num_population;
    short population[2][POPULATION_SIZE_HW][GENOME_SIZE_HW]; /* types.h */

	int costs[POPULATION_SIZE_HW];

    int idx_start_keep;
	int idx_end_keep;
	int idx_start_mutate;
	int idx_end_mutate;
	int idx_start_breed;
	int idx_end_breed;
	int idx_start_breed_and_mutate;
	int idx_end_breed_and_mutate;
	int idx_start_random;
	int idx_end_random;

	int idx_end_breed_from;
	int idx_end_mutate_from;
	int idx_end_breed_mutate_from;

    int num_mutations;

    int tournament_size;
    int truncate_size;
	
	int max_generations;
	int num_parents;
};

#endif // TYPES_H
