/*
 * ABOUT_HPP: Show the about dialog
 *
 */

#ifndef MENU_HPP
#define MENU_HPP

#include <gtkmm.h>
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _get_current_dir_name
#else
    #include <unistd.h>
    #define GetCurrentDir get_current_dir_name
 #endif

#include "main_window_renderer.hpp"

#define OPEN_GLADE_FILE "windows/openWindow.glade"
#define SAVEAS_GLADE_FILE "windows/saveAsWindow.glade"
#define ABOUT_GLADE_FILE "windows/aboutWindow.glade"

#define OPEN_WIDGET "openWindow"
#define OPEN_BUTTON "openButton"

#define SAVEAS_WIDGET "saveAsWindow"
#define SAVEAS_BUTTON "saveAsButton"
#define ENTRY_FILENAME "entryFileName"

#define CANCEL_BUTTON "cancelButton"

#define ABOUT_WIDGET "aboutWindow"

class Menu
{
    private:
        MainWindowRenderer * mainWindow;

        // Open File
        Gtk::FileChooserDialog * openFile;
        Gtk::Button * openButton;
        Gtk::Button * cancelButtonOpen;

        // SaveAs File
        Gtk::FileChooserDialog * saveAsFile;
        Gtk::Entry * entryFileName;
        Gtk::Button * saveAsButton;
        Gtk::Button * cancelButtonSaveAs;

        // About
        Gtk::AboutDialog * aboutDialog;

    protected:
        void open_openButton_clicked_cb();
        void open_cancelButton_clicked_cb();
        void saveAs_saveAsButton_clicked_cb();
        void saveAs_cancelButton_clicked_cb();

    public:
        Menu(MainWindowRenderer * mainWindow);
        void open_activate_cb();
        void saveAs_activate_cb();
        void about_activate_cb();
};

#endif
