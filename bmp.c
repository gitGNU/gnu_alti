/*

BMP file structure tools

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
#include "bmp.h"



static void bmp_header_data_stream_get (unsigned char *, int, int, int);
static void dib_header_data_stream_get (unsigned char *, int, int, int, int);
static void img_data_stream_get (unsigned char *, struct map *, int);
static int bytes_per_row (int, int);
static int padding_per_row (int, int);


extern void 
bmp_create_file (struct map * map, const char * file)
{
	FILE * stream;
	unsigned char * bmp_header_data_stream;
	unsigned char * dib_header_data_stream;
	unsigned char * img_data_stream;

	int bmp_header_data_size;
	int dib_header_data_size;
	int img_data_size;
	int row_padding;

	char file_name[256];

	bmp_header_data_size = 14;
	dib_header_data_size = 40;
	img_data_size = bytes_per_row(24, map->width) * map->height * sizeof(unsigned char);
	row_padding = padding_per_row(24, map->width);
	
	bmp_header_data_stream = (unsigned char *)malloc(bmp_header_data_size);
	dib_header_data_stream = (unsigned char *)malloc(dib_header_data_size);
	img_data_stream = (unsigned char *)malloc(img_data_size);

	bmp_header_data_stream_get(bmp_header_data_stream, bmp_header_data_size, dib_header_data_size, img_data_size);
	dib_header_data_stream_get(dib_header_data_stream, dib_header_data_size, img_data_size, map->width, map->height);
	img_data_stream_get(img_data_stream, map, row_padding);
	
	strcpy(file_name, file);
	strcat(file_name, ".bmp");
	stream = fopen(file_name, "wb");
	if(stream == NULL)
		{
			printf("Unable to create file %s\n", file);
			exit(EXIT_FAILURE);
		}

	fwrite(bmp_header_data_stream, bmp_header_data_size, 1, stream);
	fwrite(dib_header_data_stream, dib_header_data_size, 1, stream);
	fwrite(img_data_stream, img_data_size, 1, stream);
	fclose(stream);

	free(img_data_stream);
	free(dib_header_data_stream);
	free(bmp_header_data_stream);	
}


static void 
bmp_header_data_stream_get (unsigned char * stream, int bmp_header_data_size, int dib_header_data_size, int img_data_size)
{
	int file_size;
	int data_offset;

	file_size = bmp_header_data_size +  dib_header_data_size + img_data_size;
	data_offset = bmp_header_data_size + dib_header_data_size;

	stream[0] = 'B';
	stream[1] = 'M';
	stream[2] = *((unsigned char *)&file_size);
	stream[3] = *((unsigned char *)&file_size+1);
	stream[4] = *((unsigned char *)&file_size+2);
	stream[5] = *((unsigned char *)&file_size+3);
	stream[6] = 0;
	stream[7] = 0;
	stream[8] = 0;
	stream[9] = 0;
	stream[10] = *((unsigned char *)&data_offset);
	stream[11] = *((unsigned char *)&data_offset+1);
	stream[12] = *((unsigned char *)&data_offset+2);
	stream[13] = *((unsigned char *)&data_offset+3);
}


static void 
dib_header_data_stream_get (unsigned char * stream, int dib_header_data_size, int img_data_size, int width, int height)
{
	short int planes;
	short int bpp;
	int pixel_array;
	int print_res_hor;
	int print_res_ver;
	int color_in_pal;
	int color_important;

	planes = 0;
	bpp = 24;
	pixel_array = 0;
	print_res_hor = 2835;
	print_res_ver = 2835;
	color_in_pal = 0;
	color_important = 0;

	stream[0] = *((unsigned char *)&dib_header_data_size);
	stream[1] = *((unsigned char *)&dib_header_data_size+1);
	stream[2] = *((unsigned char *)&dib_header_data_size+2);
	stream[3] = *((unsigned char *)&dib_header_data_size+3);
	stream[4] = *((unsigned char *)&width);
	stream[5] = *((unsigned char *)&width+1);
	stream[6] = *((unsigned char *)&width+2);
	stream[7] = *((unsigned char *)&width+3);
	stream[8] = *((unsigned char *)&height);
	stream[9] = *((unsigned char *)&height+1);
	stream[10] = *((unsigned char *)&height+2);
	stream[11] = *((unsigned char *)&height+3);
	stream[12] = *((unsigned char *)&planes);
	stream[13] = *((unsigned char *)&planes+1);
	stream[14] = *((unsigned char *)&bpp);
	stream[15] = *((unsigned char *)&bpp+1);
	stream[16] = *((unsigned char *)&pixel_array);
	stream[17] = *((unsigned char *)&pixel_array+1);
	stream[18] = *((unsigned char *)&pixel_array+2);
	stream[19] = *((unsigned char *)&pixel_array+3);
	stream[20] = *((unsigned char *)&img_data_size);
	stream[21] = *((unsigned char *)&img_data_size+1);
	stream[22] = *((unsigned char *)&img_data_size+2);
	stream[23] = *((unsigned char *)&img_data_size+3);
	stream[24] = *((unsigned char *)&print_res_hor);
	stream[25] = *((unsigned char *)&print_res_hor+1);
	stream[26] = *((unsigned char *)&print_res_hor+2);
	stream[27] = *((unsigned char *)&print_res_hor+3);
	stream[28] = *((unsigned char *)&print_res_ver);
	stream[29] = *((unsigned char *)&print_res_ver+1);
	stream[30] = *((unsigned char *)&print_res_ver+2);
	stream[31] = *((unsigned char *)&print_res_ver+3);
	stream[32] = *((unsigned char *)&color_in_pal);
	stream[33] = *((unsigned char *)&color_in_pal+1);
	stream[34] = *((unsigned char *)&color_in_pal+2);
	stream[35] = *((unsigned char *)&color_in_pal+3);
	stream[36] = *((unsigned char *)&color_important);
	stream[37] = *((unsigned char *)&color_important+1);
	stream[38] = *((unsigned char *)&color_important+2);
	stream[39] = *((unsigned char *)&color_important+3);
}



static void 
img_data_stream_get (unsigned char * stream, struct map * map, int padding)
{
	int index;
	int x;
	int y;
	int p;

	index = 0;
	
	for(y = 0; y < map->height; y++)
		{
			for(x = 0; x < map->width; x++)
				{
					if(map->point[x][y].x != 0 && map->point[x][y].y != 0)
					{
						if(map->point[x][y].z != 0)
							{
								stream[index++] = map->point[x][y].pixel->b;
								stream[index++] = map->point[x][y].pixel->g;
								stream[index++] = map->point[x][y].pixel->r;
							}
						else
							{
								stream[index++] = 255;
								stream[index++] = 0;
								stream[index++] = 0;
							}
					}
					else
					{
						stream[index++] = 127;
						stream[index++] = 0;
						stream[index++] = 0;
					}
				}
				for(p = 0; p < padding; p++)
				{
					stream[index++] = 0;
				}
		}
}


static int
bytes_per_row (int bytes_per_pixel, int pixels_per_row)
{
	return (long)floor((double)((bytes_per_pixel * pixels_per_row) + 31) / 32) * 4;
}


static int
padding_per_row (int bytes_per_pixel, int pixels_per_row)
{
	return bytes_per_row(bytes_per_pixel,  pixels_per_row) - ( pixels_per_row * bytes_per_pixel / 8);
}


