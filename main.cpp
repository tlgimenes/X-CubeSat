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
#include <gtkmm.h>
#include <sqlite3.h>
#include <libsqlitewrapped.h>
#include <iostream>
#include <istream>

#include "log.hpp"
#include "main_window_callback.hpp"
#include "menu.hpp"
#include "manager.hpp"
#include "init.hpp"
#include "defs.hpp"

/*  --------------------------------------------------------  */
/*  Flush the content in the fifo file to allow to
 *  track the most recent satellite
 */
void flush_fifo_file(const char *fileName);
/*  --------------------------------------------------------  */



/*  --------------------------------------------------------  */
/* MAIN FUNCTION
 */
int main (int argc, char *argv[])
{
    Gtk::Main kit(argc, argv);

    Manager *man;
    Terminal *term;
    
    flush_fifo_file(FIFO_FILE);

    Log::init();
    DataBase::init();
    
    Init::XCubeSat_Controller_start(&man, &term);

    MainWindowCallback * mainWindow = new MainWindowCallback(man, term);

    // Main loop for the program
    kit.run(*(mainWindow->get_main_window()));

    return 0;
}
/*  --------------------------------------------------------  */



/*  --------------------------------------------------------  */
#define GET_FIFO_LINE(garbage) \
    fifo.getline(garbage, MAX_M_SIZE); \
    fifo.getline(garbage, MAX_M_SIZE); \
    fifo.getline(garbage, MAX_M_SIZE);
/*  --------------------------------------------------------  */
void flush_fifo_file(const char *fileName)
{
    std::ifstream fifo(fileName, std::ifstream::in);
    char *garbage = new char[MAX_M_SIZE];

    if(fifo) {
        GET_FIFO_LINE(garbage);
        while(!fifo.eof()) {
            GET_FIFO_LINE(garbage);
        }
    }
    fifo.close();
}
/*  --------------------------------------------------------  */
