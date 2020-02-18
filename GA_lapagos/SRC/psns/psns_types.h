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

#ifndef PSNS_TYPES_H
#define PSNS_TYPES_H

#define NUM_PSNS_PROBLEM_TYPES 2
enum PSNS_PROBLEM_TYPES {
                            PSNS_DAT_FORMAT_BITS,
                            PSNS_DAT_FORMAT_GRAY_CODE
                            };

typedef struct psns_problem_t_t psns_problem_t;

typedef struct psns_problem_adjacency_t_t psns_problem_adjacency_t;
typedef struct xy_t_t xy_t;

struct xy_t_t
{
    double x;
    double y;
};

struct psns_problem_t_t
{
	enum PSNS_PROBLEM_TYPES problem_type;

	int num_base_stations; // num base_stations 

	int num_UEs; // num user equipments
    int num_time_steps; // num times_steps
    int map_size_x; // map size x and y
    int map_size_y;

    int *gbr; // gbr(i) i=1,...,num_UEs - guaranteed bit rate
    int *priorities; // priorities(i) i=1,...,num_UEs - who gets priority ... higher better
    xy_t **locations; // locations(j,i) x and y - i=1,...,num_UEs j=1,...,num_time_steps - location of UEs
};

typedef struct psns_problem_bits_t_t psns_problem_bits_t;
struct psns_problem_bits_t_t
{
    int num_bits_x;
    int num_bits_y;
    int num_bits_xy;
    int num_bits;
};

#endif


