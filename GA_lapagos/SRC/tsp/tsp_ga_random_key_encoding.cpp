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

#include "tsp_ga_random_key_encoding.h"

/* Prototypes */

/* local variables */
double_linked_list_with_quick_sort_t *quick_sort_list_head;
 
/*-------------------------------------------------------------------------
 * (function: swap)
 * A utility function to swap two elements 
 *-----------------------------------------------------------------------*/
void swap ( int* a, int* b, int *idx_a, int *idx_b ) 
{ 
	int t = *a; 
	int idx_t = *idx_a; 

	*a = *b; 
	*b = t; 

	*idx_a = *idx_b; 
	*idx_b = idx_t; 
} 
  
/*-------------------------------------------------------------------------
 * (function: find_lst_node)
 * A utility function to find last node of linked list 
 *-----------------------------------------------------------------------*/
double_linked_list_with_quick_sort_t *find_last_node(double_linked_list_with_quick_sort_t *root) 
{ 
	while (root && root->next) 
		root = root->next; 
	return root; 
} 

/*-------------------------------------------------------------------------
 * (function: partition)
 * Considers last element as pivot, places the  
 * pivot element at its correct position in sorted array,  
 * and places all smaller (smaller than pivot) to left 
 * of pivot and all greater elements to right of pivot 
 * A recursive implementation of quicksort for linked list 
 *-----------------------------------------------------------------------*/
double_linked_list_with_quick_sort_t* partition(double_linked_list_with_quick_sort_t *l, double_linked_list_with_quick_sort_t *h) 
{ 
	/* set the pivot */
	int x = h->data; 
	
	/* i = i-1 */
	double_linked_list_with_quick_sort_t*i = l->prev;

	for (double_linked_list_with_quick_sort_t *j = l; j != h; j = j->next) 
	{ 
		if (j->data <= x) 
		{ 
			/* go to next if not end */
			i = (i == NULL) ? l : i->next; 
							        
			swap(&(i->data), &(j->data), &(i->location_idx), &(j->location_idx)); 
		} 
	}
	/* Similar to i++ */
	i = (i == NULL) ? l : i->next; 

	swap(&(i->data), &(h->data), &(i->location_idx), &(h->location_idx)); 
	return i; 
} 

/*-------------------------------------------------------------------------
 * (function: quick_sort_rec)
 * A recursive implementation of quicksort for linked list 
 *-----------------------------------------------------------------------*/
void quick_sort_rec(double_linked_list_with_quick_sort_t* l, double_linked_list_with_quick_sort_t *h) 
{ 
	if (h != NULL && l != h && l != h->next) 
	{ 
		double_linked_list_with_quick_sort_t *p = partition(l, h); 
		quick_sort_rec(l, p->prev); 
		quick_sort_rec(p->next, h); 
	} 
} 

/*-------------------------------------------------------------------------
 * (function: quick_sort)
 * Derived from: https://www.geeksforgeeks.org/quicksort-for-linked-list/
 *-----------------------------------------------------------------------*/
void quick_sort(double_linked_list_with_quick_sort_t *head) 
{ 
	/* Find last node */
	double_linked_list_with_quick_sort_t *tail = find_last_node(head); 

	/* Call the recursive QuickSort */
	quick_sort_rec(head, tail); 
} 

/*-------------------------------------------------------------------------
 * (function: quick_sort_init_list)
 *-----------------------------------------------------------------------*/
void random_key_quick_sort_init_list(int size) 
{
	double_linked_list_with_quick_sort_t *item;
	double_linked_list_with_quick_sort_t *last;
	int i;

	quick_sort_list_head = (double_linked_list_with_quick_sort_t *)malloc(sizeof(double_linked_list_with_quick_sort_t));
	quick_sort_list_head->prev = NULL;
	last = quick_sort_list_head;

	for (i = 1; i < size; i++)
	{
		item = (double_linked_list_with_quick_sort_t *)malloc(sizeof(double_linked_list_with_quick_sort_t));
		item->prev = last;
		item->next = NULL;
		last->next = item;
		last = item;
	}
}

/*-------------------------------------------------------------------------
 * (function: quick_sort_clean_list)
 *-----------------------------------------------------------------------*/
void random_key_quick_sort_clean_list() 
{
	double_linked_list_with_quick_sort_t *traverse = quick_sort_list_head;
	double_linked_list_with_quick_sort_t *to_free;

	while (traverse != NULL)
	{
		to_free = traverse;
		traverse = traverse->next;
		if (to_free != NULL)
			free(to_free);
	}
}

/*-------------------------------------------------------------------------
 * (function: quick_sort_fill_list)
 *-----------------------------------------------------------------------*/
void quick_sort_fill_list(int *code) 
{
	int i = 0;

	for (double_linked_list_with_quick_sort_t *traverse = quick_sort_list_head; traverse != NULL; traverse = traverse->next)
	{
		traverse->data = code[i];
		traverse->location_idx = i;

		i++;	
	}
}

/*-------------------------------------------------------------------------
 * (function: random_key_decode)
 * Bean 1994 - Genetic Algorithms and Random Keys for Sequencing and Optimization
 *-----------------------------------------------------------------------*/
int *random_key_decode(int *code, int size)
{
	int *permutation;
	int i;

	permutation = (int*)malloc(sizeof(int) * size);

	/* fill the linked list */
	quick_sort_fill_list(code);

	/* sort list */
	quick_sort(quick_sort_list_head);

	i = 0;
	for (double_linked_list_with_quick_sort_t *traverse = quick_sort_list_head; traverse != NULL; traverse = traverse->next)
	{
		permutation[traverse->location_idx] = i;
		i++;
	} 

	return permutation;
}


