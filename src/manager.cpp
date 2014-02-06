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

void Manager::AddSat(Glib::ustring *satName)
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

void Manager::AddScript(Glib::ustring *satName, Glib::ustring *scriptName, Glib::ustring *script, Glib::ustring *aliasList, Interpreter *inter)
{   
    try {
        this->satManagers[satName->c_str()]->AddScript(scriptName, script, aliasList, inter);
    }
    catch (std::allocator<std::unordered_map<std::string, SatManager*>> alloc) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the satellites list", __FILE__, __LINE__);
    }
}

bool Manager::existsSat(Glib::ustring *satName)
{
    if(this->satManagers.find(satName->c_str()) == this->satManagers.end()){
        return false;
    }
    else
        return true;
}

std::stringstream *Manager::GetFirstAlias(Glib::ustring satName)
{
    return this->satManagers[satName.c_str()]->GetFirstAlias();
}

std::stringstream *Manager::GetAlias(Glib::ustring satName, Glib::ustring script)
{
    return this->satManagers[satName.c_str()]->GetAlias(script);
}

Glib::RefPtr<Gtk::ListStore> *Manager::GetModelAliasList(Glib::ustring satName, Glib::ustring scriptName)
{
    std::unordered_map<std::string, SatManager*>::iterator it = this->satManagers.find(satName.c_str());

    if(it != this->satManagers.end())
        return it->second->GetModelAliasList(scriptName);
    else
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to load satellite, database broken", __FILE__, __LINE__);

    return NULL;
}

Glib::RefPtr<Gtk::TextBuffer> *Manager::GetTextBuffer(Glib::ustring satName, Glib::ustring scriptName)
{
    std::unordered_map<std::string, SatManager*>::iterator it = this->satManagers.find(satName.c_str());

    if(it != this->satManagers.end())
        return it->second->GetTextBufffer(scriptName);
    else
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to load satellite, database broken", __FILE__, __LINE__);

    return NULL;
}

Glib::RefPtr<Gtk::TreeStore> *Manager::GetModelSatsStore()
{
    return &(this->satsTreeStore);
}
