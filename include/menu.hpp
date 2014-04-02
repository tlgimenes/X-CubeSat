/*
 * ABOUT_HPP: Show the about dialog
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
        MainWindowRenderer *mainWindow;
        Manager *man;

        // Open File
        Gtk::FileChooserDialog *openFile;
        Gtk::Button            *openButton;
        Gtk::Button            *cancelButtonOpen;

        // SaveAs File
        Gtk::FileChooserDialog *saveAsFile;
        Gtk::Entry             *entryFileName;
        Gtk::Button            *saveAsButton;
        Gtk::Button            *cancelButtonSaveAs;

        // About
        Gtk::AboutDialog *aboutDialog;

    protected:
        void open_openButton_clicked_cb();
        void open_cancelButton_clicked_cb();
        void saveAs_saveAsButton_clicked_cb();
        void saveAs_cancelButton_clicked_cb();

    public:
        Menu(MainWindowRenderer * mainWindow, Manager *man);
        void open_activate_cb();
        void save_activate_cb();
        void saveAs_activate_cb();
        void about_activate_cb();
};

#endif
