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

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <errno.h>   /* Error number definitions */

#include "log.hpp"
#include "defs.hpp"

/*  --------------------------------------------------------  */
Glib::RefPtr<Gtk::Builder> *Log::dialogBuilder = NULL;
Gtk::MessageDialog *Log::warnDialog  = NULL;
Gtk::MessageDialog *Log::errorDialog = NULL;
Gtk::MessageDialog *Log::infoDialog  = NULL;
/*  --------------------------------------------------------  */
/*  --------------------------------------------------------  */
std::fstream * Log::logFile = new std::fstream(LOG_FILE, std::fstream::in | std::fstream::out | std::fstream::ate);
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Inits the log class
 */
void Log::init()
{
    Log::dialogBuilder = new Glib::RefPtr<Gtk::Builder>();
    *Log::dialogBuilder = (Gtk::Builder::create_from_file(DIALOG_WINDOW_GLADE));
    (*Log::dialogBuilder)->get_widget(WARN_DIALOG_WIDGET, Log::warnDialog);
    (*Log::dialogBuilder)->get_widget(ERROR_DIALOG_WIDGET, Log::errorDialog);
    (*Log::dialogBuilder)->get_widget(INFO_DIALOG_WIDGET, Log::infoDialog);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Creates the erros/warnings/info dialogs
 */
#define writeMessageLogFile(logStr, logMessage, file, line) \
    *(Log::logFile) << logStr; \
    if(file != NULL) *(Log::logFile) << "in file " << file; \
    if(line >= 0) *(Log::logFile) << " at the line " << line << ": "; \
    *(Log::logFile) << logMessage << std::endl;
 
#define writeMessageTerminal(logStr, logMessage, file, line) \
    std::cout  << logStr; \
    if(file != NULL) std::cout << "in file " << file; \
    if(line >= 0) std::cout  << " at the line " << line << ": "; \
    std::cout  << logMessage << std::endl;

void Log::LogWarn(logLevel level, const char* logMessage, const char* file, int line)
{
    Glib::ustring errorStr(logMessage);

    switch(level){
        case LEVEL_LOG_ERROR:
            errorStr.append(" in file ");
            errorStr.append(file); errorStr.append(" in line ");
            errorStr.append(std::to_string(line));
            Log::errorDialog->set_default_response(GTK_RESPONSE_OK);
            Log::errorDialog->set_secondary_text(errorStr);
            Log::errorDialog->set_message(strerror(errno));
            Log::errorDialog->run();
            Log::errorDialog->hide();
            writeMessageLogFile("ERROR " << strerror(errno) << "! ", logMessage, file, line);
            writeMessageTerminal("ERROR " << strerror(errno) << "! ", logMessage, file, line);
            exit(1);
            break;
        case LEVEL_LOG_WARNING:
            errorStr.append(" in file ");
            errorStr.append(file); errorStr.append(" in line ");
            errorStr.append(std::to_string(line)); 
            Log::warnDialog->set_default_response(GTK_RESPONSE_OK);
            Log::warnDialog->set_message(strerror(errno));
            Log::warnDialog->set_secondary_text(errorStr);
            Log::warnDialog->run();
            Log::warnDialog->hide();
            writeMessageLogFile("WARNING " << strerror(errno) << "! ", logMessage, file, line);
            writeMessageTerminal("WARNING " << strerror(errno) << "! ", logMessage, file, line);
            break;
        case LEVEL_LOG_INFO:
            Log::infoDialog->set_default_response(GTK_RESPONSE_OK);
            Log::infoDialog->set_secondary_text(errorStr);
            Log::infoDialog->run();
            Log::infoDialog->hide();
            writeMessageLogFile("INFO ", logMessage, file, line);
            writeMessageTerminal("INFO ", logMessage, file, line);
            break;
        default:
            *(Log::logFile) << "Error message not found ! This incident will be reported" << std::endl;
            exit(1);
    }
}
/*  --------------------------------------------------------  */
