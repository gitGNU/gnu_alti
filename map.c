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

#include <stdio.h>
#include <stdlib.h>
#include "map.h"




static void map_color_get (struct pixel24 *, float);
static int area_is_higher (struct map *, int, int, int);
static int area_is_lower (struct map *, int, int, int);



extern void 
point_init (struct point ** point, int x, int y, int z)
{
	struct point * newpoint;

	newpoint = (struct point *)malloc(sizeof(struct point));
	newpoint->x = x;
	newpoint->y = y;
	newpoint->z = z;
	newpoint->pixel = NULL;

	*point = newpoint;
}


extern void 
point_deinit (struct point * point)
{
	pixel24_deinit(point->pixel);
	free(point);
}




extern void 
map_init (struct map ** map, struct data * min, struct data * max, int width, int height, int step)
{
	struct map * newmap;
	int i;
	int j;

	newmap = (struct map *)malloc(sizeof(struct map));

	newmap->point = (struct point **)malloc(width * sizeof(struct point *));
	for(i = 0; i < width; i++)
		{
			newmap->point[i] = (struct point *)malloc(height * sizeof(struct point));
			for(j = 0; j < height; j++)
				{
					newmap->point[i][j].x = 0;
					newmap->point[i][j].y = 0;
					newmap->point[i][j].z = 0;
					newmap->point[i][j].pixel = NULL;
				}
		}	

	data_init(&newmap->min, min->x, min->y, min->z);
	data_init(&newmap->max, max->x, max->y, max->z);

	newmap->width = width;
	newmap->height = height;
	newmap->step = step;

	*map = newmap;
}


extern void 
map_deinit (struct map * map)
{
	int i;
	int j;

	data_deinit(map->max);
	data_deinit(map->min);

	for(i = 0; i < map->width; i++)
		{
			for(j = 0; j < map->height; j++)
				{
					free(map->point[i][j].pixel);
				}
			free(map->point[i]);
		}
	free(map->point);

	free(map);
}


extern int 
map_get_index (int min, int max, int step)
{
	return (max - min) / step;
}


extern void 
map_rasterize (struct map * map)
{
	struct pixel24 * pixel;
	float t;
	int x;
	int y;

/*	printf("map_rasterize\n");*/

	pixel24_init(&pixel, 0, 0, 0);

	for(y = 0; y < map->height; y++)
		{
			for(x = 0; x < map->width; x++)
				{
					t = (float)map->point[x][y].z / ((float)map->max->z - 3000.0f);
					if(t > 1.0f) t = 1.0f;
					/*
					if(map->point[x][y].z > 2000)
						{
							printf("map->point[x][y].z: %d\nmap->max->z: %d\nt: %f\n", map->point[x][y].z, map->max->z, t);
							getc(stdin);
						}
					*/
					map_color_get(pixel, t);
					pixel24_init(&map->point[x][y].pixel, pixel->r, pixel->g, pixel->b);
				}
		}

	pixel24_deinit(pixel);	
}


static void 
map_color_get (struct pixel24 * pixel, float t)
{
	/*0 + (t * (255 - 0));*/

	if(t < 0.25f)
		{
			pixel->r = 0; 
			pixel->g = 0 + ((t - 0.00f) * 4.0f * (255 - 0));
			pixel->b = 0;
		}
	else if(t < 0.5f)
		{
			pixel->r = 0 + ((t - 0.25f) * 4.0f * (255 - 0)); 
			pixel->g = 255; 
			pixel->b = 0 + ((t - 0.25f) * 4.0f * (255 - 0));
		}
	else if(t < 0.75f)
		{
			pixel->r = 255; 
			pixel->g = 255 + ((t - 0.5f) * 4.0f * (0 - 255));
			pixel->b = 255 + ((t - 0.5f) * 4.0f * (0 - 255));
		}	
	else
		{
			pixel->r = 255 + ((t - 0.75f) * 4.0f * (0 - 255)); 
			pixel->g = 0;
			pixel->b = 0;
		}
/*	else
		{
			pixel->r = 0;
			pixel->g = 0;
			pixel->b = 0;
		}	*/
}


extern void 
map_process (struct map * map, int zmin, int zmax)
{
	int x;
	int y;
	int z;
	int flag;

	for(y = 0; y < map->height; y ++)
		{
			for(x = 0; x < map->width; x++)
				{
					z = map->point[x][y].z;
					flag = 0;

					/*point.z >= zmin and point.z <= zmax*/
					if(z >= zmin && z <= zmax)
						{
							flag = 1;
						}

					/*point.z < zmin and one of surround > zmin*/
					if(flag == 0 && z < zmin)
						{
							if(area_is_higher(map, x, y, zmin) == 1)
								{
									flag = 1;
								}
						}					
	
					/*point.z > zmax and one of surround < zmax*/
					if(flag == 0 && z > zmax)
						{
							if(area_is_lower(map, x, y, zmax) == 1)
								{
									flag = 1;
								}
						}					

					/*render the pixel*/
					if(flag == 1)
						{
							if(map->point[x][y].pixel != NULL)
								{
									pixel24_deinit(map->point[x][y].pixel);
								}
							pixel24_init(&map->point[x][y].pixel, 0, 0, 127);
						}			
				}
		}
}


static int 
area_is_higher (struct map * map, int x, int y, int z)
{
	int n;
	int s;
	int e;
	int w;

	n = y + 1;
	s = y - 1;
	e = x + 1;
	w = x - 1;
	
	if(n >= map->height) n = map->height - 1;
	if(s <= 0) s = 0;
	if(e >= map->width) e = map->width - 1;
	if(w <= 0) w = 0;

	if(map->point[w][n].z > z)
		{
			return 1;
		}
	if(map->point[x][n].z > z)
		{
			return 1;
		}
	if(map->point[e][n].z > z)
		{
			return 1;
		}
	if(map->point[w][y].z > z)
		{
			return 1;
		}
	if(map->point[e][y].z > z)
		{
			return 1;
		}
	if(map->point[w][s].z > z)
		{
			return 1;
		}
	if(map->point[x][s].z > z)
		{
			return 1;
		}
	if(map->point[e][s].z > z)
		{
			return 1;
		}

	return 0;	
}

static int 
area_is_lower (struct map * map, int x, int y, int z)
{
	int n;
	int s;
	int e;
	int w;

	n = y + 1;
	s = y - 1;
	e = x + 1;
	w = x - 1;
	
	if(n >= map->height) n = map->height - 1;
	if(s <= 0) s = 0;
	if(e >= map->width) e = map->width - 1;
	if(w <= 0) w = 0;

	if(map->point[w][n].z < z)
		{
			return 1;
		}
	if(map->point[x][n].z < z)
		{
			return 1;
		}
	if(map->point[e][n].z < z)
		{
			return 1;
		}
	if(map->point[w][y].z < z)
		{
			return 1;
		}
	if(map->point[e][y].z < z)
		{
			return 1;
		}
	if(map->point[w][s].z < z)
		{
			return 1;
		}
	if(map->point[x][s].z < z)
		{
			return 1;
		}
	if(map->point[e][s].z < z)
		{
			return 1;
		}

	return 0;	
}

