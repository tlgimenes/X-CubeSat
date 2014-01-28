#include <gtkmm.h>
#include <iostream>

#include "log.hpp"
#include "menu.hpp"

void open_activate_cb()
{
    Gtk::FileChooserDialog * fc = 0;

    try{
        Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create_from_file(OPEN_GLADE_FILE);

        refBuilder->get_widget(OPEN_WIDGET, fc);

        if(!fc)
            Log::LogWarn(LEVEL_LOG_INFO, "Unable to load open window", __FILE__, __LINE__);

        fc->run();
    } 
    catch(const Glib::FileError& ex) {
        Log::LogWarn(LEVEL_LOG_WARNING, ex.what().c_str(), __FILE__, __LINE__);
    }

    delete fc;
}

void saveAs_activate_cb()
{
    Gtk::FileChooserDialog * fc = 0;

    try{
        Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create_from_file(SAVEAS_GLADE_FILE);

        refBuilder->get_widget(SAVEAS_WIDGET, fc);

        if(!fc)
            Log::LogWarn(LEVEL_LOG_INFO, "Unable to load open window", __FILE__, __LINE__);

        fc->run();
    } 
    catch(const Glib::FileError& ex) {
        Log::LogWarn(LEVEL_LOG_WARNING, ex.what().c_str(), __FILE__, __LINE__);
    }

    delete fc;
}

void about_activate_cb()
{
    Gtk::AboutDialog * aboutWindow = 0;

    try {
    //Load the GtkBuilder file and instantiate its widgets:
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create_from_file(ABOUT_GLADE_FILE);

    refBuilder->get_widget(ABOUT_WIDGET, aboutWindow);

    if(!aboutWindow)
        Log::LogWarn(LEVEL_LOG_INFO, "Unable to load about window", __FILE__, __LINE__);

    aboutWindow->run();
    }
    catch(const Glib::FileError& ex) {
        Log::LogWarn(LEVEL_LOG_WARNING, ex.what().c_str(), __FILE__, __LINE__);
    }

    delete aboutWindow;
}

