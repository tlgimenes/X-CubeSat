/*
 * CLASS MANAGER : This class is the core class of this
 * program. It gives methods to add some config files to
 * satellites, enqueue new data to send and receive data
 * from the satellites.
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

#include "manager.hpp"
#include "log.hpp"
#include "satellite.hpp"
#include "sat_manager.hpp"
#include "data_base.hpp"
#include "defs.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
Manager::Manager()
{
    this->elevation = 0.0f;
    this->tracking = NULL;
    this->modelSatsColumns = new ModelSatsColumns();

    this->satsTreeStore = Gtk::TreeStore::create(*this->modelSatsColumns);

}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Adds a new satellite
 */
void Manager::add_sat(Glib::ustring *satName)
{
    try {
        Gtk::TreeModel::Row row = *(this->satsTreeStore->append());
        Gtk::TreeModel::Path path(row);
        row[this->modelSatsColumns->col_sat_name] = satName->c_str();
        row[this->modelSatsColumns->col_script_name] = " ";

        Gtk::TreeModel::RowReference *rowRef = new Gtk::TreeModel::RowReference(this->satsTreeStore, path);

        this->satManagers.insert(std::make_pair<std::string, SatManager*>(satName->c_str(), new SatManager(new Satellite(satName), rowRef, this->modelSatsColumns)));
    }
    catch(std::allocator<std::unordered_map<std::string, Script*>> alloc) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the satellites list", __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Adds a new script to the satellite satName
 */
void Manager::add_script(Glib::ustring *satName, Glib::ustring *scriptName, Glib::ustring *script, Glib::ustring *aliasList, Interpreter *inter)
{   
    if(this->satManagers.find(satName->c_str()) != this->satManagers.end()) {
        try {
            this->satManagers[satName->c_str()]->add_script(scriptName, script, aliasList, inter);
        }
        catch (std::allocator<std::unordered_map<std::string, SatManager*>> alloc) {
            Log::LogWarn(LEVEL_LOG_WARNING, "Error in the satellites list", __FILE__, __LINE__);
        }
    }
    else {
        Log::LogWarn(LEVEL_LOG_WARNING, "The selected satellite is unavailable", __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns true if the satellite is in the hash table,
 * false otherwise
 */
bool Manager::exists_sat(Glib::ustring *satName)
{
    if(this->satManagers.find(satName->c_str()) == this->satManagers.end()){
        return false;
    }
    else
        return true;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns true if there is a script with the name
 * scriptName associated with the satellite with the 
 * name satName
 */
bool Manager::exists_script(Glib::ustring *satName, Glib::ustring *scriptName)
{
    if(exists_sat(satName))
        if(this->satManagers[satName->c_str()]->exists_script(*scriptName))
            return true;

    return false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Replace the old cell of the "alias" column with the 
 * new string newAlias
 */
void Manager::replace_alias_column_alias(Glib::ustring satName, Glib::ustring scriptName, const Glib::ustring& path, const Glib::ustring& newAlias)
{
    if(exists_sat(&satName))
        this->satManagers[satName.c_str()]->replace_alias_column_alias(scriptName, path, newAlias);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Replace the old cell of the "command" column with the 
 * new string newAlias
 */
void Manager::replace_alias_column_command(Glib::ustring satName, Glib::ustring scriptName, const Glib::ustring& path, const Glib::ustring& newAlias)
{
    if(exists_sat(&satName))
        this->satManagers[satName.c_str()]->replace_alias_column_command(scriptName, path, newAlias);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the alias of the first script in the scripts 
 * hash table
 */
std::stringstream *Manager::get_first_alias(Glib::ustring satName)
{
    return this->satManagers[satName.c_str()]->get_first_alias();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the alias of the script with name scriptName
 */
std::stringstream *Manager::get_alias(Glib::ustring satName, Glib::ustring script)
{
    return this->satManagers[satName.c_str()]->get_alias(script);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the GTK model of the alias frame list
 */
Glib::RefPtr<Gtk::ListStore> *Manager::get_model_alias_list(Glib::ustring satName, Glib::ustring scriptName)
{
    std::unordered_map<std::string, SatManager*>::iterator it = this->satManagers.find(satName.c_str());

    if(it != this->satManagers.end())
        return it->second->get_model_alias_list(scriptName);
    else
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to load satellite, database broken", __FILE__, __LINE__);

    return NULL;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the text buffer of the script with name scriptName
 */
Glib::RefPtr<Gtk::TextBuffer> *Manager::get_text_buffer(Glib::ustring satName, Glib::ustring scriptName)
{
    std::unordered_map<std::string, SatManager*>::iterator it = this->satManagers.find(satName.c_str());

    if(it != this->satManagers.end())
        return it->second->get_text_buffer(scriptName);
    else
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to load satellite, database broken", __FILE__, __LINE__);

    return NULL;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the GTK model of the sats frame list
 */
Glib::RefPtr<Gtk::TreeStore> *Manager::get_model_sats_store()
{
    return &(this->satsTreeStore);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the FIFO queue of scripts to be executed
 */
Glib::RefPtr<Gtk::ListStore>* Manager::get_scripts_priority_queue(Glib::ustring *satName)
{
    if(satManagers.find(satName->c_str()) != satManagers.end())
        return this->satManagers[satName->c_str()]->get_scripts_priority_queue();
    else {  
        Log::LogWarn(LEVEL_LOG_WARNING, "chose a satellite before doing this action", __FILE__, __LINE__);
    }
    return NULL;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Increase the priority of a script in the FIFO queue
 */
void Manager::increase_priority(int index, Glib::ustring satName)
{
    if(exists_sat(&satName)) {
        this->satManagers[satName.c_str()]->increase_priority(index);
    }
    else {
        Log::LogWarn(LEVEL_LOG_WARNING, "the satellite does not exists", __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Decrease the priority of a script in the FIFO queue
 */
void Manager::decrease_priority(int index, Glib::ustring satName)
{
    if(exists_sat(&satName)) {
        this->satManagers[satName.c_str()]->decrease_priority(index);
    }
    else {
        Log::LogWarn(LEVEL_LOG_WARNING, "the satelllite dows not exists", __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Save each content of script in the database and saves the
 * session
 */
void Manager::save()
{
    for(auto it = this->satManagers.begin(); it != this->satManagers.end(); it++) {
        DataBase::save_session(it->first, it->second->get_num_scripts(), it->second->get_save_str()->str());
        it->second->save();
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Renames the script with the name oldName with a the new 
 * name newName
 */
void Manager::rename_script(Glib::ustring *satName, Glib::ustring *oldScriptName, Glib::ustring *newScriptName)
{
    if(exists_sat(satName)) {
        this->satManagers[satName->c_str()]->rename_script(oldScriptName, newScriptName);
    }
    else
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to rename satellite because satellite not found", __FILE__, __LINE__); 
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Run the first script in the FIFO queue and set it in the
 * last position of the same queue
 */
void Manager::run_next_script(Glib::ustring satName)
{
    if(exists_sat(&satName)) {
        this->satManagers[satName.c_str()]->run_next_script();
    }
}
/*  --------------------------------------------------------  */
