/*

Maps utilities

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

#ifndef MAP_H
#define MAP_H

#include "tools.h"




struct point
{
	int x;
	int y;
	int z;
	struct pixel24 * pixel;
};


struct map
{
	struct point ** point;
	struct data * min;
	struct data * max;
	int width;
	int height;
	int step;
};


extern void point_init (struct point **, int, int, int);
extern void point_deinit (struct point *);




extern void map_init (struct map **, struct data *, struct data *, int, int, int);
extern void map_deinit (struct map *);
extern int map_get_index (int, int, int);
extern void map_rasterize (struct map *);
extern void map_process (struct map *, int, int);


#endif /* MAP_H */

