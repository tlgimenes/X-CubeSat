/* X-CubeSat Controller: Real-time communication with satellite program

 Copyright (C)  2014 - Tiago Lobato Gimenes

 Authors: Tiago Lobato Gimenes <tlgimenes@gmail.com>

 Comments, questions and bugreports should be submitted via
 https://github.com/tlgimenes/X-CubeSat
 More details can be found at the project home page:

 https://github.com/tlgimenes/X-CubeSat

 This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

 This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
along with this program; if not, visit http://www.fsf.org/
*/

#include "tools.hpp"

/*  --------------------------------------------------------  */  
/* Reads the FIFO file. The format is :
 * SatName'\n'
 * Elevation'\n'
 * Azimuth'\n'
 */
fifo_file_model *read_fifo_format(std::ifstream **fifo)
{
    char line[MAX_M_SIZE];
    fifo_file_model * fifofm = new fifo_file_model[1];
    static bool lock = true;

    /* Opens fifo file */
    if(!(*fifo)->is_open() && !lock) {
        (*fifo)->close();
        delete(*fifo);
        *fifo = new std::ifstream(FIFO_FILE);
        lock = !lock;
    }

    if(**fifo) {
        (*fifo)->getline(line, MAX_M_SIZE);
        fifofm->satName = new std::string(line);

        (*fifo)->getline(line, MAX_M_SIZE);
        fifofm->el = new std::string(line);

        (*fifo)->getline(line, MAX_M_SIZE);
        fifofm->az = new std::string(line);
    }
    else {
        fifofm->satName = NULL;
        fifofm->el      = NULL;
        fifofm->az      = NULL;
    }

    return fifofm;
}
/*  --------------------------------------------------------  */
