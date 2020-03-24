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

#include "genetic_algorithm_combinational_crossovers.h"
#include "genetic_algorithm_combinational_crossovers_lehmer_encoding.h"
#include "genetic_algorithm_bit_crossovers.h"

/* Prototypes */

/* globals */

/*---------------------------------------------------------------------------------------------
 * (function: setup_crossover_funcion)
 * Returns void*, and that's an easy way to pass a function 
* 
 * A comprehensive listing and taxonomy of crossover operators.	 We implement a small section of these
 *
 * A Survey on Crossover Operators
 * G. PAVAI and T. V. GEETHA, Anna University
 *-------------------------------------------------------------------------------------------*/
void* setup_crossover_function()
{
	switch(configuration.crossover_type)
	{
		/* PERMUTATION ENCODINGS */
		case PMX:
			return (void*)xover_pmx;
		case POSITION:
			return (void*)xover_position;
		case CX:
			return (void*)xover_cx;
		case CX2:
			return (void*)xover_cx2;
		case OX:
			return (void*)xover_ox;
		case CSR:
			return (void*)confined_swap_recombination;
		/* LEHMER ENCODINGS */
		case SPLX:
			return (void*)single_point_lehmer_xover;
		case TPLX:
			return (void*)two_point_lehmer_xover;
		case UPLX:
			return (void*)uniform_lehmer_xover;
		/* BIT ENCODINGS */
		case SINGLE_POINT:
			return (void*)xover_single_point;
		case TWO_POINT:
			return (void*)xover_two_point;
		case UNIFORM:
			return (void*)xover_uniform;
		default:
			printf("Not recognized crossover\n");
			return NULL;
	}
}

