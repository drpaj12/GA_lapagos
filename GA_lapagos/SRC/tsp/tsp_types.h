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

#ifndef TSP_TYPES_H
#define TSP_TYPES_H

#define NUM_TSP_PROBLEM_TYPES 3
enum TSP_PROBLEM_TYPES {ADJACENCY_PERMUTATION, ADJACENCY_LEHMER, CARTESIAN};

typedef struct tsp_problem_t_t tsp_problem_t;

typedef struct tsp_problem_coordinates_t_t tsp_problem_coordinates_t;
typedef struct tsp_city_t_t tsp_city_t;

typedef struct tsp_problem_adjacency_t_t tsp_problem_adjacency_t;
typedef struct tsp_problem_adjacency_hw_t_t tsp_problem_adjacency_hw_t;
typedef struct tsp_problem_adjacency_gpu_t_t tsp_problem_adjacency_gpu_t;

struct tsp_problem_t_t
{
	int num_cities;
	enum TSP_PROBLEM_TYPES problem_type;
};

struct tsp_problem_adjacency_t_t
{
	char *problem_name;
	double **adjacency_matrix;
};

struct tsp_problem_coordinates_t_t
{
	tsp_city_t **cities;
};

struct tsp_city_t_t
{
	int id;
	int x;
	int y;
};

/* replace with hardware outputs */
#define ADJACENCY_HW_SIZE 1
struct tsp_problem_adjacency_hw_t_t
{
        short adjacency_matrix[ADJACENCY_HW_SIZE][ADJACENCY_HW_SIZE];
};
struct tsp_problem_adjacency_gpu_t_t
{
        int adjacency_matrix[ADJACENCY_HW_SIZE*ADJACENCY_HW_SIZE];
};


#endif


