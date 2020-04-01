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
#include "tsp_types.h"

extern void lehmer_initialize_pop_list(int size);
extern void lehmer_clean(int size);
extern int *lehmer_encode(int *permutation, int size);
extern int *lehmer_decode(int *code, int size);
extern int *lehmer_decode_faster(int *code, int size);

#ifndef LEHMER
#define LEHMER

typedef struct linked_list_with_simple_repair_t_t linked_list_with_simple_repair_t;
struct linked_list_with_simple_repair_t_t
{
	int id;
	linked_list_with_simple_repair_t *next;
	linked_list_with_simple_repair_t *repair_next;
};

#endif

