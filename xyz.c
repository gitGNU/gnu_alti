/*

XYZ file structure utilities

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
#include "tools.h"
#include "xyz.h"




static void read_line (struct data *, const char *);
static void min_max_set (struct data *, struct data *, struct data *);
static void min_max_init (struct data *, struct data *, struct data *);
static void get_file_info (struct data *, struct data *, int *, FILE *);
static void get_file_data (struct map *, FILE *);




extern void 
map_create (struct map ** map, const char * file)
{
	struct map * newmap;
	FILE * stream;
	struct data * min;
	struct data * max;
	int width;
	int height;
	int step;
	
	stream = fopen(file, "r");

	if(stream == NULL)
		{
			printf("Unable to open file %s\n", file);
			exit(EXIT_FAILURE);
		}

	data_init(&min, 0, 0, 0);
	data_init(&max, 0, 0, 0);
	width = 0;
	height = 0;
	step = 0;
	
	get_file_info(min, max, &step, stream);

	width = map_get_index(min->x, max->x, step) + 1;
	height = map_get_index(min->y, max->y, step) + 1;

	map_init(&newmap, min, max, width, height, step);

	get_file_data(newmap, stream);

	data_deinit(max);
	data_deinit(min);

	if(stream != NULL)
		{
			fclose(stream);
		}

	*map = newmap;
}


static void 
get_file_info (struct data * min, struct data * max, int * step, FILE * stream)
{
	char buffer[32];
	struct data * data;
	int stack[2];

	data_init(&data, 0, 0, 0);
	stack[0] = min->x;
	stack[1] = min->x;

	fseek(stream, 0, 0);

	if(fgets(buffer, 32, stream) != NULL)
		{
			read_line(data, buffer);
			min_max_init(min, max, data);
			stack[0] = min->x;
		}

	while(fgets(buffer, 32, stream) != NULL)
		{
			read_line(data, buffer);
			min_max_set(min, max, data);
			
			if(data->x < stack[0])
				{
					stack[1] = stack[0];
					stack[0] = data->x;
				}
			else if(data->x < stack[1])
				{
					stack[1] = data->x;
				}
			else if(stack[0] == stack[1])
				{
					stack[1] = data->x;
				}
		}
		
	*step = stack[1] - stack[0];

	data_deinit(data);
}


static void 
get_file_data (struct map * map, FILE * stream)
{
	char buffer[32];
	struct data * data;
	int x;
	int y;

	memset(buffer, 0, 32);
	data_init(&data, 0, 0, 0);
	x = 0;
	y = 0;
				
	fseek(stream, 0, 0);

	while(fgets(buffer, 32, stream) != NULL)
		{
			read_line(data, buffer);

			x = map_get_index(map->min->x, data->x, map->step);
			y = map_get_index(map->min->y, data->y, map->step);

			map->point[x][y].x = data->x;
			map->point[x][y].y = data->y;
			map->point[x][y].z = data->z;
		}

	data_deinit(data);
}


static void
read_line (struct data * data, const char * line)
{
	char buffer[32];
	int i;
	int j;
	int k;
	int values[3];

	memset(buffer, 0, 32);
	values[0] = 0;
	values[1] = 0;
	values[2] = 0;
	i = 0;
	j = 0;
	k = 0;

	for(i = 0; i < strlen(line); i++)
		{
			if(line[i] == 9 || line[i] == 10)
				{
					buffer[j] = 10;
					values[k] = atol(buffer);			
					memset(buffer, 0, 32);
					j = 0;
					k++;
				}
			else
				{
					buffer[j] = line[i];
					j++;
				}
		}

	data->x = values[0];
	data->y = values[1];
	data->z = values[2];
}


static void 
min_max_init(struct data * min, struct data * max, struct data * data)
{
	data_set(min, data->x, data->y, data->z);
	data_set(max, data->x, data->y, data->z);
}


static void 
min_max_set (struct data * min, struct data * max, struct data * data)
{
	if(data->x < min->x)
		{
			min->x = data->x;
		}
	if(data->y < min->y)
		{
			min->y = data->y;
		}
	if(data->z < min->z)
		{
			min->z = data->z;
		}
	if(data->x > max->x)
		{
			max->x = data->x;
		}
	if(data->y > max->y)
		{
			max->y = data->y;
		}
	if(data->z > max->z)
		{
			max->z = data->z;
		}
}

