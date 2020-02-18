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

#ifndef MKP_TYPES_H
#define MKP_TYPES_H

#define NUM_MKP_PROBLEM_TYPES 2
enum MKP_PROBLEM_TYPES {
                            DAT_FORMAT_PENALTY,
                            DAT_FORMAT_FIX
                            };

typedef struct mkp_problem_t_t mkp_problem_t;

typedef struct mkp_problem_adjacency_t_t mkp_problem_adjacency_t;

struct mkp_problem_t_t
{
	enum MKP_PROBLEM_TYPES problem_type;

	int num_variables; // n
    int num_constraints; // m
    int optimal; // optimal value

    int *profit; // p(i) i=1,...,n 
    int **resource_matrix; // nbym matrix r(i,j) i=1,...,n; j=1,...,m
    int *bounds; // b(j) j=1,...,m
};

#endif


