/*
 * CLASS LOG : This class deals with the log file and
 * errors
 *
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

#ifndef LOG_HPP
#define LOG_HPP

#include <fstream>
#include <string>
#include <gtkmm.h>

typedef enum logLevel_t
{
    LEVEL_LOG_ERROR,
    LEVEL_LOG_WARNING,
    LEVEL_LOG_INFO
} logLevel;

class Log
{
    private:
        static std::fstream *logFile;
        static Glib::RefPtr<Gtk::Builder> *dialogBuilder;
        static Gtk::MessageDialog *warnDialog;
        static Gtk::MessageDialog *errorDialog;
        static Gtk::MessageDialog *infoDialog;

    public:
        static void init();
        static void LogWarn(logLevel, const char*, const char*, int);
}; 

#endif
