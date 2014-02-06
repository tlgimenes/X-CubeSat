/*
 * CLASS SAT_MANAGER : Each manager has one satellite
 * where it cans send and receive data.
 *
 */

#include "sat_manager.hpp"
#include "script.hpp"
#include "log.hpp"

/*SatManager::SatManager(Satellite *sat)
{
    this->sat = sat;
    this->row = NULL;
}*/

SatManager::SatManager(Satellite *sat, Gtk::TreeModel::RowReference *row, ModelSatsColumns *model)
{
    this->sat = sat;
    this->row = row;
    this->modelSatsColumns = model;
}

void SatManager::AddScript(Glib::ustring *name, Glib::ustring *script, Glib::ustring *aliasList, Interpreter *inter)
{
    try{
        /* Adds the script in the script hash table */
        scripts.insert(std::make_pair<std::string, Script*>(name->c_str(), new Script(name, script, aliasList, inter)));

        Glib::RefPtr<Gtk::TreeStore> storePrt = Glib::RefPtr<Gtk::TreeStore>::cast_dynamic(this->row->get_model());
        Gtk::TreeModel::Row row = *(storePrt->get_iter(this->row->get_path()));
        Gtk::TreeModel::Row childRow = *(storePrt->append(row.children()));

        childRow[this->modelSatsColumns->col_sat_name] = " ";
        childRow[this->modelSatsColumns->col_script_name] = name->c_str();
    }
    catch(std::allocator<std::unordered_map<std::string, Script*>> alloc) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the scripts list", __FILE__, __LINE__);
    }
}

void SatManager::RunScript(Glib::ustring scriptName)
{
    try{
        this->scripts[scriptName]->Run();
    } 
    catch(std::allocator<std::unordered_map<std::string, Script*>> alloc) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the scripts list", __FILE__, __LINE__);
    }
}

void SatManager::EnqueueScript(Glib::ustring scriptName)
{
    try {
        this->scriptsQueue.push(this->scripts[scriptName]);
    }
    catch(std::allocator<std::unordered_map<std::string, Script*>> alloc) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the scripts list", __FILE__, __LINE__);
    }
}

void SatManager::RunNextScript()
{
    try {
        this->scriptsQueue.front()->Run();
    }
    catch(std::allocator<std::unordered_map<std::string, Script*>> alloc) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the scripts list", __FILE__, __LINE__);
    }
}

std::stringstream *SatManager::GetFirstAlias()
{
    return this->scripts.begin()->second->GetAlias();
}

std::stringstream *SatManager::GetAlias(Glib::ustring scriptName)
{
    return this->scripts[scriptName.c_str()]->GetAlias();
}


Glib::RefPtr<Gtk::ListStore> *SatManager::GetModelAliasList(Glib::ustring scriptName)
{
    std::unordered_map<std::string, Script*>::iterator it = this->scripts.find(scriptName.c_str());

    if(it != this->scripts.end())
        return it->second->GetModelAliasList();
    else
        Log::LogWarn(LEVEL_LOG_ERROR, "Script could not be found in the database", __FILE__, __LINE__);

    return NULL;
}

Glib::RefPtr<Gtk::TextBuffer> *SatManager::GetTextBufffer(Glib::ustring scriptName)
{
    std::unordered_map<std::string, Script*>::iterator it = this->scripts.find(scriptName.c_str());

    if(it != this->scripts.end())
        return it->second->GetTextBuffer();
    else
        Log::LogWarn(LEVEL_LOG_ERROR, "TextBuffer could not be found in the database", __FILE__, __LINE__);

    return NULL;
}

Gtk::TreeModel::RowReference *SatManager::GetRowReference()
{
    return this->row;
}
