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

#include "psns_types.h"
#include "types.h"

extern population_t **psns_create_population_bits();
extern void psns_free_population_bits();
extern void psns_copy_old_genomes_bits(population_t **from, population_t **to, int start, int end);
extern void psns_cross_breed_bits(
                            void (*fptr_crossover)(void *, void *, void *, void *, int),
                            int (*fptr_selector)(),
                            void (*fptr_selector_init)(int),
                            population_t **from, 
                            population_t **to, 
                            int start, 
                            int end);
extern void psns_breed_and_mutate_bits(
                            void (*fptr_crossover)(void *, void *, void *, void *, int),
                            int (*fptr_selector)(),
                            void (*fptr_selector_init)(int),
                            population_t **from, 
                            population_t **to, 
                            int start, 
                            int end);
extern void psns_mutate_bits(population_t **from, population_t **to, int start, int end, int from_best);
extern void psns_copy_solution_bits(bstr from, bstr to);
void psns_random_new_bits(population_t **to, int start, int end);
extern void *psns_create_random_solution_bits();
