#include <gtkmm.h>
#include <iostream>

#include "log.hpp"
#include "port.hpp"
#include "main_window_callback.hpp"
#include "menu.hpp"
#include "manager.hpp"
#include "init.hpp"

// Connects all the call back signals of the mainWindow
//void ConnectSignalsMain(MainWindowRenderer * mainWindow);
// Connects all the call back signals of the top menu
//void ConnectSignalsMainMenu(MainWindowRenderer * mainWindow);

/*
 * MAIN FUNCTION
 */
int main (int argc, char *argv[])
{
    Gtk::Main kit(argc, argv);

    Manager *man;
    InOutInterface *inter;
    
    Log::init();
    
    Init::XCubeSat_Controller_start(&man, &inter);

    MainWindowCallback * mainWindow = new MainWindowCallback(man, inter);

    // Main loop for the program
    kit.run(*(mainWindow->get_main_window()));

    return 0;
}

