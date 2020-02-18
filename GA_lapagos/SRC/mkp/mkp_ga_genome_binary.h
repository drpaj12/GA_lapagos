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

#include "mkp_types.h"
#include "types.h"

extern population_t **mkp_create_population();
extern void mkp_free_population();
extern void mkp_cost_function_and_order(
        double (*fptr_cost_function)(void *)
        );
void mkp_cost_report_best(
        FILE *fout, 
		double (*fptr_cost_function)(void *)
        );
extern void mkp_copy_old_genomes(population_t **from, population_t **to, int start, int end);
extern void mkp_cross_breed(
                            void (*fptr_crossover)(void *, void *, void *, void *, int),
                            int (*fptr_selector)(),
                            void (*fptr_selector_init)(int),
                            population_t **from, 
                            population_t **to, 
                            int start, 
                            int end);
extern void mkp_breed_and_mutate(
                            void (*fptr_crossover)(void *, void *, void *, void *, int),
                            int (*fptr_selector)(),
                            void (*fptr_selector_init)(int),
                            population_t **from, 
                            population_t **to, 
                            int start, 
                            int end);
extern void mkp_mutate(population_t **from, population_t **to, int start, int end, int from_best);
extern short mkp_exit_condition();
extern void mkp_copy_solution(bstr from, bstr to);
void mkp_random_new(population_t **to, int start, int end);
extern void *mkp_create_random_solution();
