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
#include "types.h"

// in GA_lapagos.cpp
extern const char *ga_types_name[];
extern const char *exit_types_name[];
extern config_t configuration;
extern global_args_t global_args;
extern ga_pop_t genomes;
extern FILE *fresult_out;
extern FILE *ftest_out;
extern double global_best;
extern int breeding_cycles;
extern int global_index_hw;

// generator_static_memory.cpp
extern ga_hw_t ga_hw_data; 
extern ga_gpu_t ga_gpu_data; 

// in genetic_algorithm.cpp
extern short global_index;
// in genetic_algorithm_crossover.cpp
extern const char *crossover_types_name[];
// from genetic_algorith_selector.cpp
extern double total_cost;
extern const char *selector_types_name[]; 


