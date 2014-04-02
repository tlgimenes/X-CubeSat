/*
 * CLASS DATA_BASE : this class is an interface between 
 * the program and the data base
 */
/* X-CubeSat Controler: Real-time communication with satellite program

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

#ifndef DATA_BASE_HPP
#define DATA_BASE_HPP

#include <sqlite3.h>
#include <libsqlitewrapped.h>
#include <gtkmm.h>
#include <sstream>

typedef struct sci_data_t
{
    Glib::ustring *satName;
    unsigned int timeStamp;
    Glib::ustring *rawContent;
    Glib::ustring *dataNickname;
} sci_data;

typedef struct wod_data_t
{
    Glib::ustring *satName;
    unsigned int timeStamp;
    Glib::ustring *rawContent;
    Glib::ustring *dataNickname;
} wod_data;

class DataBase
{
    public:
        static void init();

        static std::stringstream *get_section(Glib::ustring key);
        static std::stringstream *get_session_script_names(Glib::ustring satName);
        static int                get_session_script_num(Glib::ustring satName);
        static std::stringstream *get_scripts(Glib::ustring scriptName);
        static std::stringstream *get_alias  (Glib::ustring scriptName);
        static std::stringstream *get_sats   (Glib::ustring key);

        static bool exists_section(Glib::ustring key);
        static bool exists_script (Glib::ustring key);
        //static bool exists_alias  (Glib::ustring key); Deprecated
        static bool exists_sats   (Glib::ustring key);
        static bool exists_file   (Glib::ustring key);

        static bool save_script(Glib::ustring scriptName, Glib::ustring scriptContent, Glib::ustring scriptAlias);
        static bool save_session(Glib::ustring satName, int numScripts, Glib::ustring scriptNames);
        
        static bool save_sci_data(sci_data data);
        static bool save_wod_data(wod_data data);
        static Glib::ustring get_content_file(Glib::ustring fileName);
};

#endif
