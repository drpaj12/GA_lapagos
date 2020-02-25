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
#include <time.h>
#include <sys/time.h>
#include "globals.h"
#include "types.h"
#include "utils.h"

/*---------------------------------------------------------------------------------------------
 * (function: return_string_in_list)
 *-------------------------------------------------------------------------------------------*/
short return_string_in_list(char *string, char **strings, int num_strings)
{
    int i;

    for (i = 0; i < num_strings; i++)
    {
        if (strcmp(string, strings[i]) == 0)
            return i;
    }

    return -1;
}

/*---------------------------------------------------------------------------------------------
 * (function: random)
 * From galib 
 *-------------------------------------------------------------------------------------------*/
// The ran2 pseudo-random number generator.  It has a period of 2 * 10^18 and
// returns a uniform random deviate on the interval (0.0, 1.0) excluding the
// end values.  idum initializes the sequence, so we create a separate seeding
// function to set the seed.  If you reset the seed then you re-initialize the
// sequence.

double random_float()
{ 
    return rand_float(); 
}
double random_float_in_range(double low, double high)
{
    double val=high-low; 
    val*=rand_float(); 
    return val+low;
}

unsigned long int next = 1;
/* Linear congruential generator for random */
void my_int_srand(int x)
{
	next = x;
}
 
int my_int_rand(void) // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}

#define IM1 2147483563L
#define IM2 2147483399L
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014L
#define IA2 40692L
#define IQ1 53668L
#define IQ2 52774L
#define IR1 12211L
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

static long idum2=123456789;
static long iy=0;
static long iv[NTAB];
static long idum=0;

void rand_float_seed(unsigned int seed) 
{
    int j;
    long k;

    idum = static_cast<long>(seed);
    if (idum == 0) 
        idum=1;
    if (idum < 0) 
        idum = -idum;
    idum2=(idum);
    for (j=NTAB+7;j>=0;j--) 
    {
        k=(idum)/IQ1;
        idum=IA1*(idum-k*IQ1)-k*IR1;
        if (idum < 0) 
            idum += IM1;
        if (j < NTAB) 
            iv[j] = idum;
    }
    iy=iv[0];
}

double rand_float() 
{
    int j;
    long k;
    double temp;

    k=(idum)/IQ1;
    idum=IA1*(idum-k*IQ1)-k*IR1;

    if (idum < 0) 
        idum += IM1;
    k=idum2/IQ2;
    idum2=IA2*(idum2-k*IQ2)-k*IR2;
    if (idum2 < 0) 
        idum2 += IM2;
    j=iy/NDIV;
    iy=iv[j]-idum2;
    iv[j] = idum;
    if (iy < 1) 
        iy += IMM1;

    if ((temp=AM*iy) > RNMX) 
        return RNMX;
    else 
        return temp;
}

#undef IM1
#undef IM2
#undef AM
#undef IMM1
#undef IA1
#undef IA2
#undef IQ1
#undef IQ2
#undef IR1
#undef IR2
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX

/* 
    Start Bit string from: https://rosettacode.org/wiki/Binary_strings#C
*/
bstr bitstr_new(int len)
{
    bstr s = (bstr)malloc(sizeof(bstr_t));
	if (len < 8) 
        len = 8;
	s->alloc = len;
	s->s = (unsigned char*)malloc(len);
	s->len = len;
	return s;
}
 
void bitstr_del(bstr s)
{
	free(s->s); 
    free(s);
}

bstr bitstr_dup(bstr src)
{
	bstr x = bitstr_new(src->len);
	memcpy(x->s, src->s, src->len);
	x->len = src->len;
	return x;
}

void bitstr_copy(bstr src, bstr des)
{
	memcpy(des->s, src->s, src->len);
	des->len = src->len;
}

#define CHAR_BITS 8
void bitstr_set(bstr src, int bitstr_location)
{
    int array_idx = bitstr_location / CHAR_BITS; 
    int bit_idx = bitstr_location % CHAR_BITS; 

    src->s[array_idx] = src->s[array_idx] | (1 << bit_idx);
}

void bitstr_clear(bstr src, int bitstr_location)
{
    int array_idx = bitstr_location / CHAR_BITS; 
    int bit_idx = bitstr_location % CHAR_BITS; 

    src->s[array_idx] = src->s[array_idx] & ~(1 << bit_idx);
}

void bitstr_flip(bstr src, int bitstr_location)
{
    int array_idx = bitstr_location / CHAR_BITS; 
    int bit_idx = bitstr_location % CHAR_BITS; 

    src->s[array_idx] = src->s[array_idx] ^ (1 << bit_idx);
}

short bitstr_test(bstr src, int bitstr_location)
{
    int array_idx = bitstr_location / CHAR_BITS; 
    int bit_idx = bitstr_location % CHAR_BITS; 

    if (src->s[array_idx] & (1 << bit_idx))
        return TRUE;
    else
        return FALSE;
}

void bitstr_bitswap(bstr src, int bitstr_location1, int bitstr_location2)
{
    int array_idx1 = bitstr_location1 / CHAR_BITS; 
    int bit_idx1 = bitstr_location1 % CHAR_BITS; 
    int array_idx2 = bitstr_location2 / CHAR_BITS; 
    int bit_idx2 = bitstr_location2 % CHAR_BITS; 

    short tmp_bit = bitstr_test(src, bitstr_location1);

    if (bitstr_test(src, bitstr_location2))
    {
        bitstr_set(src, bitstr_location1); 
    }
    else
    {
        bitstr_clear(src, bitstr_location1); 
    }

    if (tmp_bit)
    {
        bitstr_set(src, bitstr_location2); 
    }
    else
    {
        bitstr_clear(src, bitstr_location2); 
    }
}

void bitstr_bitcopy(bstr src, bstr des, int bitstr_location)
{
    if (bitstr_test(src, bitstr_location))
    {
        bitstr_set(des, bitstr_location); 
    }
    else
    {
        bitstr_clear(des, bitstr_location); 
    }
}

void bitstr_copy_substr(bstr src, bstr dest, int start, int end)
{
    int i;
    int start_array_idx = start / CHAR_BITS; 
    int start_bit_idx = start % CHAR_BITS; 
    int end_array_idx = end / CHAR_BITS; 
    int end_bit_idx = end % CHAR_BITS; 
    int current_array_idx = start_array_idx;
    int current_bit_idx = start_bit_idx;

    /* start byte */
    if (current_bit_idx != 0)
    {
        /* Copy the start bits */
        while(current_bit_idx % CHAR_BITS != 0)
        {
            if (bitstr_test(src, current_bit_idx))
            {
                bitstr_set(dest, current_bit_idx); 
            }
            else
            {
                bitstr_clear(dest, current_bit_idx); 
            }
            current_bit_idx ++;
        }
        current_array_idx ++;
    }

    /* middle bytes */
    for (i = current_array_idx; i < end_array_idx; i++)
    {
	    memcpy(&(dest->s[i]), &(src->s[i]), 1);
    }

    /* end byte */
    for (i = 0; i <= end_array_idx; i++)
    {
        if (bitstr_test(src, i))
        {
            bitstr_set(dest, i); 
        }
        else
        {
            bitstr_clear(dest, i); 
        }
    }
}

unsigned int bitstr_int(bstr src, int start, int end)
{
    int total = 0;
    int i;

    for (i = start; i < end; i++)
    {
        total = total << 1;
        if (bitstr_test(src, i))
            total += 1;
    }

    return total;
}

unsigned int bitstr_gray_int(bstr src, int start, int end)
{
    unsigned int value = bitstr_int(src, start, end);

    return gray_decode(value);
}

/* https://rosettacode.org/wiki/Gray_code#C */
unsigned int gray_decode(unsigned int n) 
{
    unsigned int p = n;
    while (n >>= 1) 
        p ^= n;
    return p;
}
unsigned int gray_encode(unsigned int n) 
{
    return n ^ (n >> 1);
}

void bitstr_print(bstr src)
{
    int i;
    int length;

    if (src->len % CHAR_BITS == 0)
    {
        length = src->len/CHAR_BITS;
    }
    else
    {
        length = src->len/CHAR_BITS + 1;
    }

    for (i = 0; i < length; i++)
    {
        printf("%x:", src->s[i]);
    }
    printf("\n");
}
        
#if 0 // comment out if using utrace
/* functions for the -finstrument-functions to trace code to help build hls module */
extern "C" {
static FILE *fp_trace;
void __attribute__ ((constructor)) trace_begin (void)
{
    fp_trace = fopen("trace.out", "w");
}
void __attribute__ ((destructor)) trace_end (void)
{
    if(fp_trace != NULL) 
    {
        fclose(fp_trace);
    }
}
void __cyg_profile_func_enter (void *,  void *) __attribute__((no_instrument_function)); 
void __cyg_profile_func_enter (void *func,  void *caller) 
{
    if(fp_trace != NULL) 
    {
        fprintf(fp_trace, "e %p %p %lu\n", func, caller, time(NULL) );
    }
}
 
void __cyg_profile_func_exit (void *, void *) __attribute__((no_instrument_function)); 
void __cyg_profile_func_exit (void *func, void *caller)
{
    if(fp_trace != NULL) 
    {
        fprintf(fp_trace, "x %p %p %lu\n", func, caller, time(NULL));
    }
}
}
#endif

/* In seconds */
double get_wall_time()
{
    struct timeval time;
    if (gettimeofday(&time,NULL))
    {
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
/* In seconds */
double get_cpu_time()
{
    return (double)clock() / CLOCKS_PER_SEC;
}
