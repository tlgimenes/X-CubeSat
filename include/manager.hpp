/*
 * CLASS MANAGER : This class is the core class of 
 * this program. It gives methods to add some config 
 * files to satellites, enqueue new data to send and 
 * receive data from the satellites.
 *
 */
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

#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <string>
#include <unordered_map>

#include "sat_manager.hpp"
#include "models.hpp"

class Manager
{
    private:
        std::unordered_map<std::string, SatManager*> satManagers;
        Glib::ustring *tracking;
        float elevation;
        
        /* Gtk attributes */
        Glib::RefPtr<Gtk::TreeStore> satsTreeStore;
        ModelSatsColumns *modelSatsColumns;

    public:
        Manager();

        void add_sat(Glib::ustring *satName);

        void add_script(Glib::ustring *satName, Glib::ustring *scriptName, Glib::ustring *script, Glib::ustring *aliasList, Interpreter *inter);

        void replace_alias_column_alias(Glib::ustring satName, Glib::ustring scriptName, const Glib::ustring& path, const Glib::ustring& newAlias);
        void replace_alias_column_command(Glib::ustring satName, Glib::ustring scriptName, const Glib::ustring& path, const Glib::ustring& newAlias);

        bool exists_sat(Glib::ustring *satName);
        bool exists_script(Glib::ustring *satName, Glib::ustring *scriptName);

        Glib::RefPtr<Gtk::TextBuffer> *get_text_buffer(Glib::ustring satName, Glib::ustring script);
        Glib::RefPtr<Gtk::ListStore>  *get_model_alias_list(Glib::ustring satName, Glib::ustring script);
        Glib::RefPtr<Gtk::TreeStore>  *get_model_sats_store();

        std::stringstream *get_first_alias(Glib::ustring satName);
        std::stringstream *get_alias(Glib::ustring satName, Glib::ustring scriptName);

        Glib::RefPtr<Gtk::ListStore> *get_scripts_priority_queue(Glib::ustring *satName);

        void increase_priority(int index, Glib::ustring satName);
        void decrease_priority(int index, Glib::ustring satName);

        void save(Glib::ustring sessionFile);
        void save();
        void rename_script(Glib::ustring *satName, Glib::ustring *oldScriptName, Glib::ustring *newScriptName);

        void run_next_script(Glib::ustring satName);
};

#endif
