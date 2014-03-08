/*
 * CLASS SCRIPT : Each script have an interpreter 
 * which cans run the script.
 *
 */

#include  <boost/format.hpp>
#include <sstream>

#include "script.hpp"
#include "log.hpp"

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


Glib::RefPtr<Gtk::ListStore> *Script::get_model_alias_list()
{
    return &(this->modelAliasList);
}

std::stringstream *Script::get_alias()
{
    std::stringstream *atoss = new std::stringstream();

    for(auto it=this->alias->begin(); it != this->alias->end(); it++) {
        *(atoss) << it->first << '\n' << it->second << '\n';
    }

    return atoss;
}

Glib::ustring *Script::get_name()
{
    return this->name;
}

Glib::RefPtr<Gtk::TextBuffer> *Script::get_text_buffer()
{
    return &(this->textBuffer);
}

void Script::append(Glib::ustring txt)
{
    this->textBuffer->insert_at_cursor(txt);
}

/* rever */
void Script::run()
{
    this->logList.push_back(this->interpreter->interpret(script, alias));
}

void Script::save()
{
    std::fstream script(this->name->c_str(), std::fstream::out);

    // Save the script content
    if(script.is_open())
        script << textBuffer->get_text();

    // Save the alias table content
    Glib::ustring aliasName = this->name->c_str();
    aliasName.append(".alias");

    std::fstream alias(aliasName.c_str(), std::fstream::out);

    if(alias.is_open())
        for(auto it = this->alias->begin(); it != this->alias->end(); it++) {
            alias << it->first.c_str()  << '\n';
            alias << it->second.c_str() << '\n';
        }

    script.close();
    alias.close();
}

void Script::rename(Glib::ustring *newName)
{
    if(newName != NULL && newName->size() > 0)
        this->name = newName;
    else
        Log::LogWarn(LEVEL_LOG_WARNING, "Unable to rename script", __FILE__, __LINE__);
}

void Script::replace_alias_column_alias(const Glib::ustring& path, const Glib::ustring& newAlias){
    Gtk::TreePath treePath(path);
    Gtk::ListStore::Row it;
    Glib::ustring command;
    Glib::ustring oldAlias;

    if(treePath) {
        it = *(this->modelAliasList->get_iter(treePath));

        if(it) {
            // Refresh the hash table
            oldAlias = it[this->modelAliasColumns.col_alias];
            command = (*this->alias)[oldAlias.c_str()];

            this->alias->erase(oldAlias.c_str());

            std::pair<std::string, std::string> pair (newAlias.c_str(), command);
            this->alias->insert(pair);

            // Refresh the listStore of alias
            it[this->modelAliasColumns.col_alias] = newAlias.c_str();
        }
    }
}

void Script::replace_alias_column_command(const Glib::ustring& path, const Glib::ustring& newCommand)
{
    Glib::ustring new_alias("new_alias");
    Glib::ustring new_command("new_command");
    Glib::ustring oldAlias;
    Glib::ustring oldCommand;

    Gtk::TreePath treePath(path);
    Gtk::ListStore::Row it;

    if(treePath) {
        it = *(this->modelAliasList->get_iter(treePath));

        if(it) {
            oldAlias = it[this->modelAliasColumns.col_alias];

            // Refresh alias hash table
            oldCommand = (*this->alias)[oldAlias.c_str()];
            (*this->alias)[oldAlias.c_str()] = newCommand.c_str();

            // add new row if necessary
            if(!oldCommand.compare(new_command) && newCommand.compare(new_command)) {
                Gtk::TreeModel::Row newRow = *this->modelAliasList->append();

                if(newRow) {
                    newRow.set_value(this->modelAliasColumns.col_alias, new_alias);
                    newRow.set_value(this->modelAliasColumns.col_command, new_command);
                    // Refresh alias hash table
                    std::pair<std::string, std::string> pair(new_alias.c_str(), new_command.c_str());
                    this->alias->insert(pair);
                }
            }
            // Set the new value in the treeModel
            it[this->modelAliasColumns.col_command] = newCommand.c_str();
        }
    }
}
