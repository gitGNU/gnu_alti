/*

alti.

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
#include "xyz.h"
#include "bmp.h"
#include "tools.h"




int
main (int argc, char * argv[])
{
	struct input_argument * input_argument;
	struct map * map;
	
	input_argument_init(&input_argument);
	input_argument_parse(input_argument, argc, argv);
	input_argument_print(input_argument);

	map_create(&map, input_argument->file_name_input);

	map_rasterize(map);

	map_process(map, input_argument->alti_min, input_argument->alti_max);

	bmp_create_file(map, input_argument->file_name_output);

	map_deinit(map);

	input_argument_deinit(input_argument);

	return 0;
}

