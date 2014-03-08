/*
 * CLASS SCRIPT : Each script have an interpreter 
 * which cans run the script.
 *
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
        void save();
        void run();
};

#endif
