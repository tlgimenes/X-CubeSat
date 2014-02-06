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
#include "alias.hpp"

#define MAX_ALIAS_SIZE 999

class Script
{
    private:
        Glib::ustring *name;
        Glib::ustring *script;
        std::unordered_map<std::string, std::string> *alias;
        std::vector<InOutLog*> logList;

        Interpreter *interpreter;

        /* TreeView of the Alias Frame */
        ModelAliasColumns modelAliasColumns;
        Glib::RefPtr<Gtk::ListStore> modelAliasList;

        /* TextView of the Command File frame */
        Glib::RefPtr<Gtk::TextBuffer> textBuffer;

    public:
        Script(Glib::ustring *name, Glib::ustring *script, Glib::ustring *aliasList, Interpreter *inter);
        Glib::RefPtr<Gtk::ListStore> *GetModelAliasList();
        Glib::RefPtr<Gtk::TextBuffer> *GetTextBuffer();
        std::stringstream *GetAlias();
        Glib::ustring *GetName();

        void Append(Glib::ustring txt);
        void Run();
};

#endif
