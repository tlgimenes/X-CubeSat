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

#include <iostream>

#include "sat_manager.hpp"
#include "script.hpp"
#include "log.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
SatManager::SatManager(Satellite *sat, Gtk::TreeModel::RowReference *row, ModelSatsColumns *model)
{
    this->sat = sat;
    this->row = row;
    this->modelSatsColumns = model;

    this->scriptsPriorityQueue = Gtk::ListStore::create(this->modelScriptsPriorityQueue); 
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Add a new script to this satellite manager. The given
 * interpreter is used to interpret the script that is
 * being added giving the possibility of writing different
 * interpreters to different satellites and scripts
 */
void SatManager::add_script(Glib::ustring *name, Glib::ustring *script, Glib::ustring *aliasList, Interpreter *inter)
{
    if(scripts.find(name->c_str()) == scripts.end()) {
        try{
            /* Adds the script in the script hash table */
            scripts.insert(std::make_pair<std::string, Script*>(name->c_str(), new Script(name, script, aliasList, inter)));

            /* Adds scripts in the sats frame */
            Glib::RefPtr<Gtk::TreeStore> storePrt = Glib::RefPtr<Gtk::TreeStore>::cast_dynamic(this->row->get_model());
            Gtk::TreeModel::Row row = *(storePrt->get_iter(this->row->get_path()));
            Gtk::TreeModel::Row childRow = *(storePrt->append(row.children()));

            childRow[this->modelSatsColumns->col_sat_name] = " ";
            childRow[this->modelSatsColumns->col_script_name] = name->c_str();

            /* Adds the script in the script queue */
            this->scriptsQueue.push_back(scripts[name->c_str()]);
            Gtk::ListStore::Row newRow = *(this->scriptsPriorityQueue->append());
            newRow[this->modelScriptsPriorityQueue.col_script_name] = name->c_str();
        }
        catch(std::allocator<std::unordered_map<std::string, Script*>> alloc) {
            Log::LogWarn(LEVEL_LOG_WARNING, "Error in the scripts list", __FILE__, __LINE__);
        }
    }
    else {
        Log::LogWarn(LEVEL_LOG_WARNING, "New script not added because it already exists", __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* run the script of name scriptName
 */
void SatManager::run_script(Glib::ustring scriptName)
{
    try{
        this->scripts[scriptName]->run(this->sat->get_nickname());
    } 
    catch(std::allocator<std::unordered_map<std::string, Script*>> alloc) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the scripts list", __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Inserts in the last position of the FIFO queue the script
 * with name scriptName to be executed
 */
void SatManager::enqueue_script(Glib::ustring scriptName)
{
    try {
        this->scriptsQueue.push_back(this->scripts[scriptName]);
    }
    catch(std::allocator<std::unordered_map<std::string, Script*>> alloc) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the scripts list", __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Run the first script in the FIFO queue and set it in the
 * last position of the same queue
 */
void SatManager::run_next_script()
{
    try {
        if(!this->scriptsQueue.empty()) {
            this->scriptsQueue.front()->run(this->sat->get_nickname());
            this->scriptsQueue.push_back(this->scriptsQueue.front());
            this->scriptsQueue.erase(this->scriptsQueue.begin());

            Gtk::ListStore::Row beg = *this->scriptsPriorityQueue->children().begin();
            Gtk::ListStore::Row row = *this->scriptsPriorityQueue->append();
            row[this->modelScriptsPriorityQueue.col_script_name] = beg->get_value(this->modelScriptsPriorityQueue.col_script_name);
            this->scriptsPriorityQueue->erase(beg);
        }
    }
    catch(std::allocator<std::unordered_map<std::string, Script*>> alloc) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the scripts list", __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns true if the script exists, false in the other way
 */
bool SatManager::exists_script(Glib::ustring name)
{
    if(this->scripts.find(name.c_str()) != this->scripts.end())
        return true;
    else
        return false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Renames the script with the name oldName with a the new 
 * name newName
 */
void SatManager::rename_script(Glib::ustring *oldName, Glib::ustring *newName)
{
    Script *script;
    unsigned int it;

    if(exists_script(*oldName)) {
        // removes from the hash table
        script = this->scripts[oldName->c_str()];
        this->scripts.erase(oldName->c_str());
        
        // refresh the name in the priority
        // queue treeview model
        for(it=0; it < this->scriptsQueue.size(); it++) 
            if(this->scriptsQueue[it] == script) 
                break;
        if(it < this->scriptsQueue.size()) {
            Gtk::TreePath path(std::to_string(it));
            Gtk::ListStore::Row row = *(this->scriptsPriorityQueue->get_iter(path));
            row[this->modelScriptsPriorityQueue.col_script_name] = newName->c_str();
        }
                
        // reinsert the new renamed script
        std::pair<std::string, Script*> pair(newName->c_str(), script);
        this->scripts.insert(pair);

        script->rename(newName);
    }
    else
        Log::LogWarn(LEVEL_LOG_WARNING, "Unable to rename script because it was not found", __FILE__, __LINE__);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the alias of the first script in the scripts 
 * hash table
 */
std::stringstream *SatManager::get_first_alias()
{
    return this->scripts.begin()->second->get_alias();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the alias of the script with name scriptName
 */
std::stringstream *SatManager::get_alias(Glib::ustring scriptName)
{
    return this->scripts[scriptName.c_str()]->get_alias();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the GTK model of the alias frame list
 */
Glib::RefPtr<Gtk::ListStore> *SatManager::get_model_alias_list(Glib::ustring scriptName)
{
    std::unordered_map<std::string, Script*>::iterator it = this->scripts.find(scriptName.c_str());

    if(it != this->scripts.end())
        return it->second->get_model_alias_list();
    else
        Log::LogWarn(LEVEL_LOG_ERROR, "Script could not be found in the database", __FILE__, __LINE__);

    return NULL;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the text buffer of the script with name scriptName
 */
Glib::RefPtr<Gtk::TextBuffer> *SatManager::get_text_buffer(Glib::ustring scriptName)
{
    std::unordered_map<std::string, Script*>::iterator it = this->scripts.find(scriptName.c_str());

    if(it != this->scripts.end())
        return it->second->get_text_buffer();
    else
        Log::LogWarn(LEVEL_LOG_ERROR, "TextBuffer could not be found in the database", __FILE__, __LINE__);

    return NULL;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the row in the Sats frame where this object 
 * appears
 */
Gtk::TreeModel::RowReference *SatManager::get_row_reference()
{
    return this->row;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the FIFO queue of scripts to be executed
 */
Glib::RefPtr<Gtk::ListStore>* SatManager::get_scripts_priority_queue()
{
    return &(this->scriptsPriorityQueue);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Prints the FIFO queue. For debug only purposes
 */
void print_priority_queue(std::vector<Script*> q)
{
    int count = 0;

    for(Script* it : q) {
        std::cout << "scriptQueue[" << count << "] = " << it->get_name()->c_str() << std::endl;
        count++;
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Increase the priority of a script in the FIFO queue
 */
void SatManager::increase_priority(int index)
{
    Script *aux;

    if(index > -1 && (unsigned int)index < this->scriptsQueue.size()) {
        if(index > 0) {
            aux = this->scriptsQueue[index];
            this->scriptsQueue[index] = this->scriptsQueue[index-1];
            this->scriptsQueue[index-1] = aux;
        }
    }
    else {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the priority queue", __FILE__, __LINE__);
    }

    //print_priority_queue(this->scriptsQueue);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Decrease the priority of a script in the FIFO queue
 */
void SatManager::decrease_priority(int index)
{
    Script *aux;

    if(index > -1 && (unsigned int)index < this->scriptsQueue.size()) {
        if((unsigned int)index < this->scriptsQueue.size() -1) {
            aux = this->scriptsQueue[index];
            this->scriptsQueue[index] = this->scriptsQueue[index+1];
            this->scriptsQueue[index+1] = aux;
        }
    }
    else {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the priority queue", __FILE__, __LINE__);
    }

    //print_priority_queue(this->scriptsQueue);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Sets the pointer of fs to a string str in the file 
 */
bool set_stream_ptr(std::fstream *fs, Glib::ustring str)
{
    char cmp[MAX_SAT_NAME_SIZE];

    while(!fs->eof()) {
        fs->getline(cmp, MAX_SAT_NAME_SIZE);
        if(!str.compare(cmp)) {
            return true;
        }
    }
    return false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Transform the scripts hash table in a string with the 
 * name of each script per line to save it in the database
 */
std::stringstream *SatManager::get_save_str()
{
    std::stringstream *sst = new std::stringstream();

    for(unsigned int i = 0; i < this->scriptsQueue.size(); i++) {
        *sst << *this->scriptsQueue[i]->get_name() << '\n';
    }
    return sst;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Save each content of script in the database
 */
void SatManager::save()
{
     for(unsigned int i = 0; i < this->scriptsQueue.size(); i++) {
         this->scriptsQueue[i]->save();
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Replace the old cell of the "alias" column with the 
 * new string newAlias
 */
void SatManager::replace_alias_column_alias(Glib::ustring scriptName, const Glib::ustring& path, const Glib::ustring& newAlias)
{
    if(exists_script(scriptName))
        this->scripts[scriptName.c_str()]->replace_alias_column_alias(path, newAlias);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Replace the old cell of the "command" column with the
 * new string newAlias
 */
void SatManager::replace_alias_column_command(Glib::ustring scriptName, const Glib::ustring& path, const Glib::ustring& newAlias)
{
    if(exists_script(scriptName))
        this->scripts[scriptName.c_str()]->replace_alias_column_command(path, newAlias);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the number of scripts associated with this 
 * satellite manager
 */
int SatManager::get_num_scripts()
{
    return this->scripts.size();
}
/*  --------------------------------------------------------  */
