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

extern short return_string_in_list(char *string, char **strings, int num_strings);

extern double random_float();
extern double random_float_in_range(double low, double high);
extern void rand_float_seed(unsigned int seed);
extern double rand_float();
extern void my_int_srand(int x);
extern int my_int_rand(void); // RAND_MAX assumed to be 32767

extern bstr bitstr_new(int len);
extern void bitstr_del(bstr s);
extern bstr bitstr_dup(bstr src);
extern void bitstr_copy(bstr src, bstr des);
extern void bitstr_set(bstr src, int bitstr_location);
extern void bitstr_clear(bstr src, int bitstr_location);
extern void bitstr_flip(bstr src, int bitstr_location);
extern short bitstr_test(bstr src, int bitstr_location);
extern void bitstr_bitcopy(bstr src, bstr des, int bitstr_location);
extern void bitstr_bitswap(bstr src, int bitstr_location1, int bitstr_location2);
extern void bitstr_copy_substr(bstr src, bstr dest, int start, int end);
extern void bitstr_print(bstr src);
extern unsigned int bitstr_int(bstr src, int start, int end);
extern unsigned int bitstr_gray_int(bstr src, int start, int end);

extern unsigned int gray_decode(unsigned int n) ;
extern unsigned int gray_encode(unsigned int n) ;
