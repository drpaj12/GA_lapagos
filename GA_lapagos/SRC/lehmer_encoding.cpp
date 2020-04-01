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

/* An explanation about Lehmer Codes: from Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * An algorithm for manipulating and generating permutations in lexicographic
 * order by using the factoradic number system.
 *
 * The factoradic number system (also called the factorial number system) is a
 * way of writing out numbers in a base system that depends on factorials,
 * rather than powers of numbers.  In the factoradic system, the last digit is
 * always 0 and is in base 0!.  The digit before that is either 0 or 1 and is
 * in base 1!.  The digit before that is either 0, 1, or 2 and is in base 2!.
 * More generally, the nth-to-last digit in always 0, 1, 2, ..., or n and is in
 * base n!.  When writing a number, we usually append a subscript ! to indicate
 * that it's written in factoradic base.  In this (plain-text) C++ source file,
 * I'll denote this by suffixing the number with _!.
 *
 * As an example, the factoradic number 2110_! is equal to
 *
 *      2 x 3! + 1 x 2! + 1 x 1! + 0 x 0!
 *    = 2 x 6  + 1 x 2  + 1 x 1  + 0 x 1
 *    =    12  +     2  +     1  +     0
 *    =    15
 *
 * Similarly, the first few factoradic numbers are
 *
 *     0_!  10_!  100_!  110_!  200_!  210_!  1000_!  1010_!  1100_!  1110_!
 *
 * One reason that the factoradic numbers are so useful is that there's a close
 * connection between factoradic numbers and permutations of distinct values.
 * To understand where this comes from, suppose that we're interested in
 * generating all of the permutations of some collection in lexicographic
 * order.  One way to do this might be to think about generating the first such
 * permutation, then the second, then the third, etc.  In other words, we want
 * to look for some way of mapping the integers onto permutations in a way that
 * allows us to efficiently answer the question "what is the nth permutation of
 * this set?"
 *
 * Let's consider permutations of four elements - a, b, c, and d - and the
 * order in which they're generated.  Here's a table of these permutations:
 *
 *               0  abcd       6 bacd      12 cabd      18 dabc
 *               1  abdc       7 badc      13 cadb      19 dacb
 *               2  acbd       8 bcad      14 cbad      20 dbac
 *               3  acdb       9 bcda      15 cbda      21 dbca
 *               4  adbc      10 bdac      16 cdab      22 dcab
 *               5  adcb      11 bdca      17 cdba      23 dcba
 *
 * Initially, there might not seem to be any pattern relating the numbers to
 * the permutations, but there are a few trends we can spot here.  For example,
 * every sixth permutation starts with a new letter, and 6 = 3!.  The second
 * letter of each permutation changes every two permutations, and 2 = 2!.
 *
 * If we rewrite the above table with the integers in factoradic base, then a
 * more obvious trend starts to appear:
 *
 *          0000_!  abcd  1000_! bacd  2000_! cabd  3000_! dabc
 *          0010_!  abdc  1010_! badc  2010_! cadb  3010_! dacb
 *          0100_!  acbd  1100_! bcad  2100_! cbad  3100_! dbac
 *          0110_!  acdb  1110_! bcda  2110_! cbda  3110_! dbca
 *          0200_!  adbc  1200_! bdac  2200_! cdab  3200_! dcab
 *          0210_!  adcb  1210_! bdca  2210_! cdba  3210_! dcba
 *
 * We noted above that the first letter changes every six permutations, and now
 * we can see that this coincides with the times at which the first digit of
 * the factoradic representation changes.  Similarly, when we noted that the
 * second letter of the permutation changed every two permutations, we can see
 * this is occurring whenever the second digit of the factoradic representation
 * of the index is changing.
 *
 * The connection here is due to a construction called "Lehmer codes," a way of
 * labeling permutations in a way that also allows their construction.  The
 * idea behind Lehmer codes is as follows.  Suppose that you are given a
 * permutation of a totally-ordered set of elements, such as the permutation
 * BEDAC of the letters A-E.  We start off by looking at the first letter of
 * this permutation - in this case, B - and seeing how many elements it's
 * bigger than (in this case, 1).  We then split this letter off of the
 * permutation and record the fact that it was bigger than one element:
 *
 *     B   EDAC
 *     1
 *
 * Now, we repeat this process on EDAC.  Here, E is larger than three of the
 * elements of EDAC (everything except itself), so we write out a three to get
 *
 *     BE  DAC
 *     13
 *
 * Repeating this on D gives us
 *
 *     BED  AC
 *     132
 *
 * And on A gives us
 *
 *     BEDA  C
 *     1320  0
 *
 * And finally on the last C gives us
 *
 *     BEDAC
 *     13200
 *
 * So the Lehmer code for this permutation is (1, 3, 2, 0, 0).  Note that the
 * length of a Lehmer code is equal to the number of elements in the
 * permutation.
 *
 * Given a permutation's Lehmer code, we can generate the permutation it stands
 * for by running this process backwards.  Suppose, for example, that we're
 * given the Lehmer code (3, 1, 0, 0) and want to use it to recover some given
 * permutation of ABCD.  To do this, we'd start off by taking the element of
 * this collection of letters that's bigger than three other elements.  This is
 * equivalent to asking which element is at position 3 using zero-indexing, and
 * gives us D.  So now we have
 *
 *     D    ABC
 *     3
 *
 * and the remaining Lehmer code (1, 0, 0).  Looking at the 1 in the first
 * position of this Lehmer code, we should take the element out of the unused
 * elements at position 1 (which, zero-indexed, is B), giving us
 *
 *     DB   AC
 *     31
 *
 * The rest of our code is (0, 0), so we take the zeroth-largest element of the
 * unused letters (A) and put it next:
 *
 *     DBA  C
 *     310
 *
 * And finally, we use the last part of our code, (0), to select the last
 * element of the permutation, which is C:
 *
 *     DBAC
 *     3100
 *
 * The clincher in all this is that there's a simple, one-to-one mapping
 * between the factoradic numbers and Lehmer codes.  The idea is simple - given
 * a Lehmer code (d0, d1, ..., dn), we just pick the number
 *
 *                                d0 d1 d2 ... dn_!
 *
 * For example, given Lehmer code (3, 1, 0, 0), we'd pick the factoradic number
 * 3100_!.  Similarly, given factoradic number 210_!, we'd pick the Lehmer code
 * (2, 1, 0).
 *
 * In order to convince ourselves that this is even mathematically well-defined
 * we need to see that each of the digits we assign are within the range
 * specified for that digit.  For example, if it were possible that we could
 * have a Lehmer code (1, 4), then we couldn't convert it to the factoradic
 * number 14_!, since the last digit of any factoradic number must be 0.  This
 * is not particularly difficult to see.  Remember that the digits of the
 * Lehmer code are zero-based indices into the list of elements we haven't yet
 * used yet.  Thus the last digit must be zero, since there's only one element
 * remaining; the penultimate digit can be either one or zero since there's
 * two possible choices; the antepenultimate digit can be zero, one or two;
 * etc.
 *
 * Given the bijiective correspondence between Lehmer codes and the factoradic
 * numbers, we have a way of taking a natural number N and mapping it into a
 * permutation of some number of elements.  However, as of now we have no
 * reason to believe that this will list every permutation in lexicographic
 * order.  After all, given some random encoding system for permutations, why
 * should we expect anything special out of our ordering?  Fortunately, though
 * we have the following lemma:
 *
 * Lemma: Given two permutations p_1 and p_2 with Lehmer codes L_1 and L_2,
 * p_1 lexicographically precedes p_2 iff L_1 lexicographically precedes L_2.
 *
 * Corollary: Every permutation has a unique Lehmer code.
 *
 * Before we go into the proof, I should remark that this guarantees that the
 * following algorithm will list all permutations of n elements in order:
 *
 * For i = 0 to n! - 1:
 *    Represent i in factoradic base.
 *    Convert this representation to a Lehmer code.
 *    Generate the permutation based on this Lehmer code.
 *
 * This algorithm works correctly because every permutation has a Lehmer code,
 * and by iterating over all n! different codes we'll hit every permutation.
 * Moreover, our lemma guarantees that no two permutations have the same
 * Lehmer code, we know that we'll get every permutation in order and exactly
 * once.
 *
 * Proof of lemma: We first prove that if p_1 < p_2, then L_1 < L_2.  Given the
 * two permutations, consider the first elements at which the permutations
 * disagree, call this index i.  Note that i can't be the last element of the
 * permutations, because that would mean that the permutations agree in every
 * position except the last, and since the last element doesn't match it means
 * that the permutations aren't of the same elements.  Since the permutations
 * agree on the first i - 1 elements, the Lehmer codes for the first i - 1
 * elements must agree.  This means that at the time the ith digit of the
 * Lehmer code is being chosen, in both permutations the same set of leftover
 * elements will remain.  Since p_1 < p_2, the ith element of p_1 must be
 * smaller than the ith element of p_2, and so the index of that element in
 * what's left must be smaller than the index of the corresponding element from
 * p_2 in what's left.  Consequently, the digit chosen for L_1 will be less
 * than the digit for L_2, and since the codes agree on their prefix before
 * this we have that L_1 < L_2.
 *
 * To prove the opposite direction, suppose that L_1 < L_2 and find the first
 * spot at which they disagree; say it's index i.  i cannot be the last spot
 * in the code, since the very last digit must be a 0, so it has to be some
 * place before that.  Since the prefixes of the codes match up to that point,
 * at the spot where L_1 chooses a lower number than L_2, the partial
 * permutations built up so far must match and the remaining elements must be
 * the same.  Consequently, when L_1 has a lower value than L_2, the resulting
 * permutation will have a smaller value in p_1 than in p_2, and since the
 * prefixes match the result will be that p_1 < p_2.  QED.
 *
 * The functions exported by this file allow for the generation and
 * manipulation of permutations using factoradic number representations.  They
 * run in quadratic time, though faster implementations are possible using more
 * complex algorithms.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "globals.h"
#include "types.h"

#include "lehmer_encoding.h"

/* Prototypes */

/*-------------------------------------------------------------------------
 * (function: lehmer_encode)
 *  From : Ucoluk, Gokturk - Genetic Algorithm Solution of the TSP Avoiding Special Crossover Mutation
 *-----------------------------------------------------------------------*/
int *lehmer_encode(int *permutation, int size)
{
	int *inversion;	
	int i, j;

	inversion = (int*)malloc(sizeof(int) * size);

	for (i = 0; i < size; i++)
	{
		inversion[i] = 0;
		j = 0;

		while (permutation[j] != i)
		{
			if (permutation[j] > i)
			{
				inversion[i]++;
			}
			j ++;
		}
	}

	return inversion;
}

/*-------------------------------------------------------------------------
 * (function: lehmer_decode)
 *  From : Ucoluk, Gokturk - Genetic Algorithm Solution of the TSP Avoiding Special Crossover Mutation
 *  
 *  Could also implement with a pop from list data structure...might be faster
 *-----------------------------------------------------------------------*/
int *lehmer_decode(int *code, int size)
{
	int *permutation;
	int *position;
	int i, j;

	permutation = (int*)malloc(sizeof(int) * size);
	position = (int*)malloc(sizeof(int) * size);

	for (i = size -1; i >=0; i--)
	{
		for (j = i+1; j < size; j++)
		{
			if (position[j] >= code[i])
			{
				position[j]++;
			}
		}
		position[i] = code[i];
	}

	for (i = 0 ; i < size; i++)
	{
		permutation[position[i]] = i;
	}

	free(position);

	return permutation;
}

linked_list_with_simple_repair_t *list_head;
/*-------------------------------------------------------------------------
 * (function: initialize_pop_list)
 *-----------------------------------------------------------------------*/
void lehmer_initialize_pop_list(int size)
{
	int i;

	linked_list_with_simple_repair_t_t *allocated_next;
	linked_list_with_simple_repair_t_t *last;

	/* create the head */
	allocated_next = (linked_list_with_simple_repair_t *)malloc(sizeof(linked_list_with_simple_repair_t));
	allocated_next->id = 0;

	last = allocated_next;
	list_head = allocated_next;

	/* create the pop list */
	for (i = 1; i < size; i++)
	{
		/* make next */
		allocated_next = (linked_list_with_simple_repair_t *)malloc(sizeof(linked_list_with_simple_repair_t));
		allocated_next->id = i;
		/* attach to the last */
		last->next = allocated_next;
		last->repair_next = allocated_next;

		last = allocated_next;
	}

	/* terminate the list */
	last->next = NULL;
	last->repair_next = NULL;
}

/*-------------------------------------------------------------------------
 * (function: repair_list)
 * Fixes all the links as we popped out before
 *-----------------------------------------------------------------------*/
void repair_list(int size)
{
	linked_list_with_simple_repair_t *traverse;

	for (traverse = list_head; traverse->repair_next != NULL; traverse = traverse->repair_next)
	{
		traverse->next = traverse->repair_next;
	}
}

/*-------------------------------------------------------------------------
 * (function: pop_eleement_X_from_list)
 * Take out the Xth element
 *-----------------------------------------------------------------------*/
int pop_element_X_from_list(int X)
{
	int count = 0;

	linked_list_with_simple_repair_t_t *traverse;
	linked_list_with_simple_repair_t_t *previous = NULL;

	for (traverse = list_head; traverse != NULL; traverse = traverse->next)
	{
		/* when found */
		if (count == X)
		{
			/* if head then pop out */
			if (list_head == traverse)
				list_head == traverse->next;
			else
				previous->next = traverse->next;

			/* return the item */
			return traverse->id;
		}

		/* count and record where we are */
		count ++;
		previous = traverse;
	}

	return -1;
}

/*-------------------------------------------------------------------------
 * (function: lehmer_clean)
 *-----------------------------------------------------------------------*/
void lehmer_clean(int size)
{
	int i;

	linked_list_with_simple_repair_t_t *traverse;
	linked_list_with_simple_repair_t_t *to_free;

	traverse = list_head;

	/* create the pop list */
	for (i = 0; i < size; i++)
	{
		to_free = traverse;
		traverse = traverse->repair_next;
		free(to_free);
	}
}

/*-------------------------------------------------------------------------
 * (function: lehmer_decode_fast)
 * Uses https://en.wikipedia.org/wiki/Lehmer_code second suggestion
 * for decoding b popping elements off a list
 *-----------------------------------------------------------------------*/
int *lehmer_decode_faster(int *code, int size)
{
	int *permutation;
	int i, j;

	/* we reuse a list, so repair it from last popping actions */
	repair_list(size);

	permutation = (int*)malloc(sizeof(int) * size);

	/* pop the i from the code next into permutation and do until list all codes moved */
	for (i = 0; i < size; i++)
	{
		permutation[i] = pop_element_X_from_list(code[i]);
	}

	return permutation;
}
