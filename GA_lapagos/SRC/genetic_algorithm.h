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

void ga_init(population_t **(*fptr_init_starting_population)(void));
void ga_clean(void (*fptr_clean_population)(void));

int run_base_ga(
        int (*fptr_selector)(),
        void (*fptr_selector_init)(int),
		void (*fptr_cost_function_order)(double (*)(void *)),
		double (*fptr_cost_function)(void *),
		void (*fptr_copy_old_over)(population_t**, population_t**, int, int),
		void (*fptr_cross_breed)(
                                        void (*)(void *, void *, void *, int), 
                                        int (*)(),
                                        void (*)(int),
                                        population_t**, population_t**, int, int),
		void (*fptr_mutate)(population_t**, population_t**, int, int, int),
		void (*fptr_mutate_no_copy)(population_t**, population_t**, int, int, int),
		void (*fptr_random_new)(population_t**, int, int),
		void (*fptr_cost_report_best)(FILE *, double (*)(void *)),
		void (*fptr_crossover)(void *, void *, void *, int),
		short (*fptr_exit_condition)(void)
		);

void output_test_details(double final_cost);
