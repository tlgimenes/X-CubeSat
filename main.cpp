#include <gtkmm.h>
#include <iostream>

#include "log.hpp"
#include "port.hpp"
#include "mainWindow.hpp"
#include "menu.hpp"
#include "manager.hpp"
#include "init.hpp"

// Connects all the call back signals of the mainWindow
void ConnectSignalsMain(MainWindow * mainWindow);
// Connects all the call back signals of the top menu
void ConnectSignalsMainMenu(MainWindow * mainWindow);

/*
 * MAIN FUNCTION
 */
int main (int argc, char *argv[])
{
    Gtk::Main kit(argc, argv);

    Manager *man;
    InOutInterface *inter;
    
    Log::Init();
    
    Init::XCubeSat_Controler_Start(&man, &inter);

    MainWindow * mainWindow = new MainWindow(man, inter);

    // Connects the call back signals
    ConnectSignalsMain(mainWindow);

    // Main loop for the program
    kit.run(*(mainWindow->get_mainWindow()));

    return 0;
}

/*
 * Connects all the signals 
 */
void ConnectSignalsMain(MainWindow * mainWindow)
{
    ConnectSignalsMainMenu(mainWindow);
}

void ConnectSignalsMainMenu(MainWindow * mainWindow)
{
    Glib::RefPtr<Gtk::Builder> refBuilder = mainWindow->get_mainBuilder();
    Gtk::ImageMenuItem * imitem = 0;
    Menu * menu = new Menu(mainWindow);

    refBuilder->get_widget("saveFileAs", imitem);
    if(imitem != 0)
        imitem->signal_activate().connect(sigc::mem_fun(menu, &Menu::saveAs_activate_cb));

    refBuilder->get_widget("openFile", imitem);
    if(imitem != 0)
        imitem->signal_activate().connect(sigc::mem_fun(menu, &Menu::open_activate_cb));

    refBuilder->get_widget("quit", imitem);
    if(imitem != 0)
        imitem->signal_activate().connect(sigc::ptr_fun(gtk_main_quit));

    refBuilder->get_widget("about", imitem);
    if(imitem != 0)
        imitem->signal_activate().connect(sigc::mem_fun(menu, &Menu::about_activate_cb));
}
