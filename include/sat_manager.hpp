/*
 * CLASS SAT_MANAGER : Each manager has one satellite
 * where it cans send and receive data.
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
#ifndef SAT_MANAGER_HPP
#define SAT_MANAGER_HPP

#include <unordered_map>
#include <queue>
#include <string>

#include "satellite.hpp"
#include "script.hpp"
#include "interpreter.hpp"
#include "models.hpp"

#define MAX_SAT_NAME_SIZE 400

class SatManager
{
    private:
        Satellite * sat;

        std::unordered_map<std::string, Script*> scripts;

        std::vector<Script*>         scriptsQueue;
        Glib::RefPtr<Gtk::ListStore> scriptsPriorityQueue;
        ModelScriptsPriorityQueue    modelScriptsPriorityQueue;

        Gtk::TreeModel::RowReference *row;
        ModelSatsColumns             *modelSatsColumns;

    public:
        SatManager(Satellite *sat, Gtk::TreeModel::RowReference *row, ModelSatsColumns *model);

        Glib::RefPtr<Gtk::TextBuffer> *get_text_buffer(Glib::ustring script);
        Glib::RefPtr<Gtk::ListStore>  *get_model_alias_list(Glib::ustring script);
        Gtk::TreeModel::RowReference  *get_row_reference();
        std::stringstream             *get_first_alias();
        std::stringstream             *get_alias(Glib::ustring script);
        Glib::RefPtr<Gtk::ListStore>  *get_scripts_priority_queue();

        void add_script(Glib::ustring *name, Glib::ustring *script, Glib::ustring *aliasList, Interpreter *inter);

        void replace_alias_column_alias(Glib::ustring scriptName, const Glib::ustring& path, const Glib::ustring& newAlias);
        void replace_alias_column_command(Glib::ustring scriptName, const Glib::ustring& path, const Glib::ustring& newAlias);

        void enqueue_script(Glib::ustring scriptName);
        void run_script(Glib::ustring scriptName);
        void run_next_script();
        void rename_script(Glib::ustring *oldName, Glib::ustring *newName);

        void increase_priority(int index);
        void decrease_priority(int index);

        bool exists_script(Glib::ustring name);

        void save();
        std::stringstream *get_save_str();
        int get_num_scripts();
};

#endif
