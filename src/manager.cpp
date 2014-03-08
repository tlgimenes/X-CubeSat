/*
 * CLASS MANAGER : This class is the core class of this
 * program. It gives methods to add some config files to
 * satellites, enqueue new data to send and receive data
 * from the satellites.
 *
 */

#include "manager.hpp"
#include "log.hpp"
#include "satellite.hpp"
#include "sat_manager.hpp"

Manager::Manager()
{
    this->elevation = 0.0f;
    this->tracking = NULL;
    this->modelSatsColumns = new ModelSatsColumns();

    this->satsTreeStore = Gtk::TreeStore::create(*this->modelSatsColumns);
}

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

bool Manager::exists_sat(Glib::ustring *satName)
{
    if(this->satManagers.find(satName->c_str()) == this->satManagers.end()){
        return false;
    }
    else
        return true;
}

bool Manager::exists_script(Glib::ustring *satName, Glib::ustring *scriptName)
{
    if(exists_sat(satName))
        if(this->satManagers[satName->c_str()]->exists_script(*scriptName))
            return true;

    return false;
}

void Manager::replace_alias_column_alias(Glib::ustring satName, Glib::ustring scriptName, const Glib::ustring& path, const Glib::ustring& newAlias)
{
    if(exists_sat(&satName))
        this->satManagers[satName.c_str()]->replace_alias_column_alias(scriptName, path, newAlias);
}

void Manager::replace_alias_column_command(Glib::ustring satName, Glib::ustring scriptName, const Glib::ustring& path, const Glib::ustring& newAlias)
{
    if(exists_sat(&satName))
        this->satManagers[satName.c_str()]->replace_alias_column_command(scriptName, path, newAlias);
}

std::stringstream *Manager::get_first_alias(Glib::ustring satName)
{
    return this->satManagers[satName.c_str()]->get_first_alias();
}

std::stringstream *Manager::get_alias(Glib::ustring satName, Glib::ustring script)
{
    return this->satManagers[satName.c_str()]->get_alias(script);
}

Glib::RefPtr<Gtk::ListStore> *Manager::get_model_alias_list(Glib::ustring satName, Glib::ustring scriptName)
{
    std::unordered_map<std::string, SatManager*>::iterator it = this->satManagers.find(satName.c_str());

    if(it != this->satManagers.end())
        return it->second->get_model_alias_list(scriptName);
    else
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to load satellite, database broken", __FILE__, __LINE__);

    return NULL;
}

Glib::RefPtr<Gtk::TextBuffer> *Manager::get_text_buffer(Glib::ustring satName, Glib::ustring scriptName)
{
    std::unordered_map<std::string, SatManager*>::iterator it = this->satManagers.find(satName.c_str());

    if(it != this->satManagers.end())
        return it->second->get_text_buffer(scriptName);
    else
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to load satellite, database broken", __FILE__, __LINE__);

    return NULL;
}

Glib::RefPtr<Gtk::TreeStore> *Manager::get_model_sats_store()
{
    return &(this->satsTreeStore);
}

Glib::RefPtr<Gtk::ListStore>* Manager::get_scripts_priority_queue(Glib::ustring *satName)
{
    if(satManagers.find(satName->c_str()) != satManagers.end())
        return this->satManagers[satName->c_str()]->get_scripts_priority_queue();
    else {  
        Log::LogWarn(LEVEL_LOG_WARNING, "chose a satellite before doing this action", __FILE__, __LINE__);
    }
    return NULL;
}

void Manager::increase_priority(int index, Glib::ustring satName)
{
    if(exists_sat(&satName)) {
        this->satManagers[satName.c_str()]->increase_priority(index);
    }
    else {
        Log::LogWarn(LEVEL_LOG_WARNING, "the satellite does not exists", __FILE__, __LINE__);
    }
}

void Manager::decrease_priority(int index, Glib::ustring satName)
{
    if(exists_sat(&satName)) {
        this->satManagers[satName.c_str()]->decrease_priority(index);
    }
    else {
        Log::LogWarn(LEVEL_LOG_WARNING, "the satelllite dows not exists", __FILE__, __LINE__);
    }
}

void Manager::save(Glib::ustring sessionFile)
{
    std::fstream fs(sessionFile.c_str(), std::fstream::out);
    std::stringstream scratch;

    for(auto it = this->satManagers.begin(); it != this->satManagers.end(); it++) {
        scratch << it->first;
        scratch << it->second->get_save_str(sessionFile)->str() << std::endl;
    }

    fs << scratch.str().c_str();

fs.close();
}

void Manager::rename_script(Glib::ustring *satName, Glib::ustring *oldScriptName, Glib::ustring *newScriptName)
{
    if(exists_sat(satName)) {
        this->satManagers[satName->c_str()]->rename_script(oldScriptName, newScriptName);
    }
    else
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to rename satellite because satellite not found", __FILE__, __LINE__); 
}
