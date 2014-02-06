/*
 * CLASS SCRIPT : Each script have an interpreter 
 * which cans run the script.
 *
 */

#include  <boost/format.hpp>
#include <sstream>

#include "script.hpp"

Script::Script(Glib::ustring *name, Glib::ustring *script, Glib::ustring *aliasList, Interpreter *inter)
{
    char alias[MAX_ALIAS_SIZE];
    char command[MAX_ALIAS_SIZE];
    this->name = name;
    this->script = script;
    this->interpreter = inter;
    this->modelAliasList = Gtk::ListStore::create(this->modelAliasColumns);

    /* Creates alias hash table and adds data to the model*/
    this->alias = new std::unordered_map<std::string, std::string>();
    std::stringstream aliasStr(aliasList->c_str());

    while(!aliasStr.eof()) {
        aliasStr.getline(alias, MAX_ALIAS_SIZE);
        aliasStr.getline(command, MAX_ALIAS_SIZE);

        /* Adds to hash table */
        this->alias->insert(std::make_pair<std::string, std::string>(alias, command));

        /* Adds to the model */
        Gtk::TreeModel::Row row = *(this->modelAliasList->append());
        row[this->modelAliasColumns.col_alias] = alias;
        row[this->modelAliasColumns.col_command] = command;

        aliasStr.peek();
    }

    /* Adds the script to the textBuffer */
    this->textBuffer = Gtk::TextBuffer::create();
    this->textBuffer->set_text(script->c_str());
}


Glib::RefPtr<Gtk::ListStore> *Script::GetModelAliasList()
{
    return &(this->modelAliasList);
}

std::stringstream *Script::GetAlias()
{
    std::stringstream *atoss = new std::stringstream();

    for(auto it=this->alias->begin(); it != this->alias->end(); it++) {
        *(atoss) << it->first << '\n' << it->second << '\n';
    }

    return atoss;
}

Glib::ustring *Script::GetName()
{
    return this->name;
}

Glib::RefPtr<Gtk::TextBuffer> *Script::GetTextBuffer()
{
    return &(this->textBuffer);
}

void Script::Append(Glib::ustring txt)
{
    this->textBuffer->insert_at_cursor(txt);
}

/* rever */
void Script::Run()
{
    this->logList.push_back(this->interpreter->Interpret(script, alias));
}
