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

int setup_selector_function_hw();
void setup_selector_init_function_hw(int value);

extern void selector_roulette_init_hw(int num_selecting);
extern int selector_roulette_hw();
extern void selector_sus_init_hw(int num_selecting);
extern int selector_sus_hw();
extern void selector_tournament_init_hw(int num_selecting);
extern int selector_tournament_hw();
extern void selector_rank_init_hw(int num_selecting);
extern int selector_rank_hw();
extern void selector_random_init_hw(int num_selecting);
extern int selector_random_hw();
extern void selector_truncate_init_hw(int num_selecting);
extern int selector_truncate_hw();
