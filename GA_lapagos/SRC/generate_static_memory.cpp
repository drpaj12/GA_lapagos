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

#include "tsp_static_memory_generator.h"

/* Prototypes */

/* globals */
ga_hw_t ga_hw_data; 
ga_gpu_t ga_gpu_data; 

/*---------------------------------------------------------------------------------------------
 * (function: generate_static_variables)
 *-------------------------------------------------------------------------------------------*/
void generate_static_variables(
							short mode,
							FILE *file_output,
							int (*fptr_genome_size)(),
							void (*fptr_genome_type)(FILE *),
							void (*fptr_genome2d)(FILE *, population_t **),
							void (*fptr_genome1d)(FILE *, population_t **))
{
	/* only TSP right now */
	tsp_generate_hw_variables(mode,
								file_output,
								fptr_genome_size,
								fptr_genome_type,
								fptr_genome2d,
								fptr_genome1d);
}

