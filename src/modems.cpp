/* X-CubeSat Controller: Real-time communication with satellite program

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

#include "modems.hpp"
#include "defs.hpp"
#include "log.hpp"

/* --------------------------------------------------------------*/
Modem::Modem(modemModel_t model, Gtk::ComboBoxText *name, Gtk::ComboBoxText *mode)
{
    try {
        this->modemNameCombobox = name;
        this->modemModeCombobox = mode;

        switch(model) 
        {
            case DEFAULT:
            case KANTRONICS:
            case KENWOOD:
                this->currentModem = model;
                break;
            default:
                Log::LogWarn(LEVEL_LOG_WARNING, "wrong modem configuration, setting to default", __FILE__, __LINE__);
                this->currentModem = DEFAULT;
        }

        this->populate_name_combobox();
        this->populate_mode_combobox();
    }
    catch(const Gtk::BuilderError &ex) {
        std::cout << ex.what() << std::endl;
    }
    catch(const std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
}
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
void Modem::populate_name_combobox()
{
    modemNameCombobox->remove_all();
    for (size_t i = 0; i < modems.size(); ++i) {
        modemNameCombobox->append(modems[i]->name().c_str());
    }
    modemNameCombobox->set_active(0);
}
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
void Modem::populate_mode_combobox()
{
    std::vector<std::string> modes = modems[currentModem]->modes();
    modemModeCombobox->remove_all();
    for (size_t i = 0; i < modes.size(); ++i) {
        modemModeCombobox->append(modes[i].c_str());
    }
    modemModeCombobox->set_active(0);
}
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
void Modem::update_modem_name()
{
    size_t i;
    Glib::ustring modemName = this->modemNameCombobox->get_active_text();

    for (i = 0; modemName.compare(modems[i]->name()) && i < modems.size(); ++i) {
        currentModem = static_cast<modemModel_t>(i);
    }
    currentModem = static_cast<modemModel_t>(i);

    if(i >= modemName.size()) {
        Log::LogWarn(LEVEL_LOG_WARNING, "wrong modem configuration, setting to default", __FILE__, __LINE__);
        this->currentModem = DEFAULT;
    }

    this->populate_mode_combobox();
}
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
/*void Modem::modem_mode_changed_combobox_cb()
{
    Glib::ustring modeName = this->modemModeCombobox->get_active_text();

    this->modems[currentModem]->change_mode(modeName);
}*/
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
std::string Modem::REPLY_OEM()
{
    return this->modems[currentModem]->REPLY_OEM();
}
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
/* TODO: REMOVE THIS FUNCTION */
std::string Modem::OEM()
{
    return "\r";
}
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
void Modem::set_change_modem_mode_cb(sigc::slot<void> change_mode_cb)
{
    this->modemModeCombobox->signal_changed().connect(change_mode_cb);
}
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
void Modem::set_change_modem_name_cb(sigc::slot<void> change_name_cb)
{
    this->modemNameCombobox->signal_changed().connect(change_name_cb);
}
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
Glib::ustring Modem::update_modem_mode()
{
    Glib::ustring modeName = this->modemModeCombobox->get_active_text();

    return this->modems[currentModem]->change_modem_mode(modeName);
}
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
Glib::ustring Modem::format_send(Glib::ustring str)
{
    return this->modems[currentModem]->format_send(str);
}
/* --------------------------------------------------------------*/
