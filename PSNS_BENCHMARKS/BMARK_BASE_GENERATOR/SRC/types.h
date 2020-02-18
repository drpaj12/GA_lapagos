/*

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

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TYPES_H
#define TYPES_H

typedef struct global_args_t_t global_args_t;

//#define oassert(x) {if(!(x)){exit(-1);}} // causes an interrupt in GDB
#define oassert(x) {if(!(x)){__asm("int3");}} // causes an interrupt in GDB

/* the global arguments of the software */
struct global_args_t_t
{
	char *output_file;
	char *way_point_file;

	int num_base_stations; // num base_stations 

	int num_UEs; // num user equipments
    int num_time_steps; // num times_steps
    int time_steps_in_waypoints;
    int map_size_x; // map size x and y
    int map_size_y;
};

#endif // TYPES_H

