/*

Tools utilities

This file is part of alti.

Copyright (C) 2015 slughnaz
 
alti is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

alti is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
   
You should have received a copy of the GNU General Public License
along with alti.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "tools.h"




extern void 
data_init (struct data ** data, int x, int y, int z)
{
	struct data * newdata;

	newdata = (struct data *)malloc(sizeof(struct data));
	newdata->x = x;
	newdata->y = y;
	newdata->z = z;

	*data = newdata;
}


extern void 
data_deinit (struct data * data)
{
	free(data);
}


extern void 
data_set(struct data * data, int x, int y, int z)
{
	data->x = x;
	data->y = y;
	data->z = z;
}




extern void 
pixel24_init (struct pixel24 ** pixel, int r, int g, int b)
{
	struct pixel24 * newpixel;

	newpixel = (struct pixel24 *)malloc(sizeof(struct pixel24));
	newpixel->r = r;
	newpixel->g = g;
	newpixel->b = b;

	*pixel = newpixel;
}


extern void 
pixel24_deinit (struct pixel24 * pixel)
{
	free(pixel);
}




extern void 
input_argument_init (struct input_argument ** input_argument)
{
	struct input_argument * newinput_argument;

	newinput_argument =  (struct input_argument *)malloc(sizeof(struct input_argument));

	*input_argument = newinput_argument;
}

extern void 
input_argument_deinit (struct input_argument * input_argument)
{
	free(input_argument);
}

extern void 
input_argument_parse (struct input_argument * input_argument, int argc, char ** argv)
{
	strncpy(input_argument->prog_name, argv[0], 256);	
	strncpy(input_argument->file_name_input, argv[1], 256);
	strncpy(input_argument->file_name_output, argv[2], 256);
	input_argument->alti_min = atoi(argv[3]);
	input_argument->alti_max = atoi(argv[4]);
}

extern void
input_argument_print (struct input_argument * input_argument)
{
	printf("input_argument->prog_name: %s\n", input_argument->prog_name);
	printf("input_argument->file_name_input: %s\n", input_argument->file_name_input);
	printf("input_argument->file_name_output: %s\n", input_argument->file_name_output);
	printf("input_argument->alti_min: %u\n", input_argument->alti_min);
	printf("input_argument->alti_max: %u\n", input_argument->alti_max);
}


extern void 
print_time (char * label)
{
	char * s;
	time_t t;

	t = time(NULL);
	s = ctime(&t);
	printf("%s : %s", label, s);
}

