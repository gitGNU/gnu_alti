
# alti makefile. 

# This file is part of alti.

# Copyright (C) 2015 slughnaz
 
# alti is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# alti is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
   
# You should have received a copy of the GNU General Public License
# along with alti.  If not, see <http://www.gnu.org/licenses/>.


application: alti.o bmp.o xyz.o tools.o map.o
	gcc -o alti alti.o bmp.o xyz.o tools.o map.o -lm

bmp.o: bmp.c
	gcc -c bmp.c -ansi -Wall -pedantic -ggdb

xyz.o: xyz.c
	gcc -c xyz.c -ansi -Wall -pedantic -ggdb

tools.o: tools.c
	gcc -c tools.c -ansi -Wall -pedantic -ggdb

map.o: map.c
	gcc -c map.c -ansi -Wall -pedantic -ggdb

alti.o: alti.c
	gcc -c alti.c -ansi -Wall -pedantic -ggdb
