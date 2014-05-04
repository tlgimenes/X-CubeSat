/*
 * CLASS SCRIPT : Each script have an interpreter 
 * which cans run the script.
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
#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <gtkmm.h>

#include "in_out_log.hpp"
#include "interpreter.hpp"
#include "models.hpp"

#define MAX_ALIAS_SIZE 999

class Script
{
    private:
        std::unordered_map<std::string, std::string> *alias;
        std::vector<InOutLog*> logList;
        Glib::ustring          *name;
        Glib::ustring          *script;

        Interpreter *interpreter;

        /* TreeView of the Alias Frame */
        Glib::RefPtr<Gtk::ListStore> modelAliasList;
        ModelAliasColumns            modelAliasColumns;

        /* TextView of the Command File frame */
        Glib::RefPtr<Gtk::TextBuffer> textBuffer;

    public:
        Script(Glib::ustring *name, Glib::ustring *script, Glib::ustring *aliasList, Interpreter *inter);
        Glib::RefPtr<Gtk::ListStore>    *get_model_alias_list();
        Glib::RefPtr<Gtk::TextBuffer>   *get_text_buffer();
        std::stringstream               *get_alias();
        Glib::ustring                   *get_name();

        void append(Glib::ustring txt);
        void rename(Glib::ustring *newName);
        void replace_alias_column_alias(const Glib::ustring& path, const Glib::ustring& newAlias);
        void replace_alias_column_command(const Glib::ustring& path, const Glib::ustring& new_text);
        void save();
        void run(Glib::ustring *satName);
};

#endif
