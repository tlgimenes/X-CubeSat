/*
 * CLASS LOG : This class deals with the log file and
 * errors
 *
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
            Log::warnDialog->set_message(strerror(errno));
            Log::warnDialog->set_secondary_text(errorStr);
            Log::warnDialog->run();
            Log::warnDialog->hide();
            writeMessageLogFile("WARNING " << strerror(errno) << "! ", logMessage, file, line);
            writeMessageTerminal("WARNING " << strerror(errno) << "! ", logMessage, file, line);
            break;
        case LEVEL_LOG_INFO:
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
