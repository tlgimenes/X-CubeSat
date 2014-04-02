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

#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "log.hpp"
#include "menu.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
Menu::Menu(MainWindowRenderer * mainWindow, Manager *man)
{
    try {
        this->mainWindow = mainWindow;
        this->man = man;

        Glib::RefPtr<Gtk::Builder> openBuilder   = Gtk::Builder::create_from_file(OPEN_GLADE_FILE);
        Glib::RefPtr<Gtk::Builder> saveAsBuilder = Gtk::Builder::create_from_file(SAVEAS_GLADE_FILE);
        Glib::RefPtr<Gtk::Builder> aboutBuilder  = Gtk::Builder::create_from_file(ABOUT_GLADE_FILE);

        /*
         * Open File
         */
        openBuilder->get_widget(OPEN_WIDGET, this->openFile);
        openBuilder->get_widget(OPEN_BUTTON, this->openButton);
        openBuilder->get_widget(CANCEL_BUTTON, this->cancelButtonOpen);
        openButton->signal_clicked().connect(sigc::mem_fun(this, &Menu::open_openButton_clicked_cb));
        cancelButtonOpen->signal_clicked().connect(sigc::mem_fun(this, &Menu::open_cancelButton_clicked_cb));

        /*
         * SaveAs File
         */
        saveAsBuilder->get_widget(SAVEAS_WIDGET, this->saveAsFile);
        saveAsBuilder->get_widget(SAVEAS_BUTTON, this->saveAsButton);
        saveAsBuilder->get_widget(CANCEL_BUTTON, this->cancelButtonSaveAs);
        saveAsBuilder->get_widget(ENTRY_FILENAME, this->entryFileName);
        saveAsButton->signal_clicked().connect(sigc::mem_fun(this, &Menu::saveAs_saveAsButton_clicked_cb));
        cancelButtonSaveAs->signal_clicked().connect(sigc::mem_fun(this, &Menu::saveAs_cancelButton_clicked_cb));

        /*
         * About
         */
        aboutBuilder->get_widget(ABOUT_WIDGET, this->aboutDialog);
    }
    catch(const Glib::FileError& ex) {
        Log::LogWarn(LEVEL_LOG_WARNING, ex.what().c_str(), __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void Menu::open_openButton_clicked_cb()
{
    std::ifstream file(this->openFile->get_filename().c_str());
    std::ostringstream out;

    if(file.is_open()) {
        out << file.rdbuf();

        this->mainWindow->get_text_editor()->get_buffer()->set_text(out.str());
 
        this->openFile->hide();
    }
    else {
        Log::LogWarn(LEVEL_LOG_WARNING, "File could not be oppened", __FILE__, __LINE__);
    }

    file.close();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void Menu::open_cancelButton_clicked_cb()
{
    if(this->openFile == 0)
        Log::LogWarn(LEVEL_LOG_WARNING, "Cancel Button panic !", __FILE__, __LINE__);

    this->openFile->hide();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void Menu::open_activate_cb()
{
    if(!this->openFile)
        Log::LogWarn(LEVEL_LOG_INFO, "Unable to load open window", __FILE__, __LINE__);

    this->openFile->set_current_folder(GetCurrentDir());
    this->openFile->run();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void Menu::saveAs_saveAsButton_clicked_cb()
{
    Glib::ustring ext(".xcs");

    if(!this->saveAsFile)
        Log::LogWarn(LEVEL_LOG_WARNING, "SaveAs Button panic !", __FILE__, __LINE__);
    

    Glib::ustring fileName = this->entryFileName->get_text();
    fileName.append(ext);

    Glib::ustring pathName = this->saveAsFile->get_current_folder();
    pathName.append("/");
    pathName.append(fileName);

    std::ofstream file(pathName.c_str(), std::ios::out);

    if(file.is_open()) {
        Glib::ustring text = mainWindow->get_text_editor()->get_buffer()->get_text();

        file << text;
    }

    file.close();

    this->saveAsFile->hide();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void Menu::saveAs_cancelButton_clicked_cb()
{
    if(!this->saveAsFile)
        Log::LogWarn(LEVEL_LOG_WARNING, "Cancel Button panic !", __FILE__, __LINE__);

    this->saveAsFile->hide();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* TODO: FINISH HERE !!! 
 */
void Menu::save_activate_cb()
{
    Glib::ustring session = DEFAULT_SESSION_FILE;
    this->man->save();
}

/*  --------------------------------------------------------  */
void Menu::saveAs_activate_cb()
{
    if(!this->saveAsFile)
        Log::LogWarn(LEVEL_LOG_INFO, "Unable to load open window", __FILE__, __LINE__);

    this->saveAsFile->set_current_folder(GetCurrentDir());
    this->saveAsFile->run();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void Menu::about_activate_cb()
{
   if(!this->aboutDialog)
        Log::LogWarn(LEVEL_LOG_INFO, "Unable to load about window", __FILE__, __LINE__);

    this->aboutDialog->run();
}
/*  --------------------------------------------------------  */
