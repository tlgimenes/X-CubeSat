#include <gtkmm.h>
#include <iostream>

#include "log.hpp"
#include "port.hpp"
#include "menu.hpp"

// Connects all the call back signals of the mainWindow
void ConnectSignalsMain(Glib::RefPtr<Gtk::Builder> refBuilder);
// Connects all the call back signals of the top menu
void ConnectSignalsMainMenu(Glib::RefPtr<Gtk::Builder> refBuilder);

int main (int argc, char *argv[])
{
    Gtk::Main kit(argc, argv);

    Gtk::Window* mainWindow = 0;

    // Load the GtkBuilder file and instantiate its widgets:
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("windows/mainWindow.glade");
        
        //Get the GtkBuilder-instantiated window:
        refBuilder->get_widget("mainWindow", mainWindow);

        if(!mainWindow)
            Log::LogWarn(LEVEL_LOG_ERROR, "The program could not open its main window", __FILE__, __LINE__);
 
        // Connects the call back signals
        ConnectSignalsMain(refBuilder);

        // Main loop for the program
        kit.run(*mainWindow);
    }
    catch(const Glib::FileError& ex) {
        Log::LogWarn(LEVEL_LOG_ERROR, ex.what().c_str(), __FILE__, __LINE__);
        return 1;
    }

    delete mainWindow;

    return 0;
}

/*
 * Connects all the signals 
 */
void ConnectSignalsMain(Glib::RefPtr<Gtk::Builder> refBuilder)
{
    ConnectSignalsMainMenu(refBuilder);
}

void ConnectSignalsMainMenu(Glib::RefPtr<Gtk::Builder> refBuilder)
{
    Gtk::ImageMenuItem * imitem = 0;

    refBuilder->get_widget("saveFileAs", imitem);
    if(imitem != 0)
        imitem->signal_activate().connect(sigc::ptr_fun(saveAs_activate_cb));

    refBuilder->get_widget("openFile", imitem);
    if(imitem != 0)
        imitem->signal_activate().connect(sigc::ptr_fun(open_activate_cb));

    refBuilder->get_widget("quit", imitem);
    if(imitem != 0)
        imitem->signal_activate().connect(sigc::ptr_fun(gtk_main_quit));

    refBuilder->get_widget("about", imitem);
    if(imitem != 0)
        imitem->signal_activate().connect(sigc::ptr_fun(about_activate_cb));

}

