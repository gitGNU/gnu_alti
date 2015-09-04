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

#ifndef TOOLS_H
#define TOOLS_H




struct data
{
	int x;
	int y;
	int z;
};

extern void data_init (struct data **, int, int, int);
extern void data_deinit (struct data *);
extern void data_set(struct data *, int, int, int);



struct pixel24
{
	int r;
	int g;
	int b;
};

extern void pixel24_init (struct pixel24 **, int, int, int);
extern void pixel24_deinit (struct pixel24 *);




struct input_argument
{
	char prog_name[256];
	char file_name_input[256];
	char file_name_output[256];
	int alti_min;
	int alti_max;
};

extern void input_argument_init (struct input_argument **);
extern void input_argument_deinit (struct input_argument *);
extern void input_argument_parse (struct input_argument *, int, char **);
extern void input_argument_print (struct input_argument *);


extern void print_time (char *);



#endif /* TOOLS_H */

