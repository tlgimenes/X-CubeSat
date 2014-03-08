/*
 * CLASS LOG : This class deals with the log file and
 * errors
 *
 */

#ifndef LOG_HPP
#define LOG_HPP

#include <fstream>
#include <string>
#include <gtkmm.h>

#define LOG_FILE "XCubeSat_Controler.txt"

#define DIALOG_WINDOW_GLADE "windows/dialogWindow.glade"
#define WARN_DIALOG_WIDGET  "warnDialog"
#define ERROR_DIALOG_WIDGET "errorDialog"
#define INFO_DIALOG_WIDGET  "infoDialog"

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
