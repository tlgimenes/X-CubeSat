/*
 * CLASS SAT_MANAGER : Each manager has one satellite
 * where it cans send and receive data.
 *
 */

#include <iostream>

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

    this->scriptsPriorityQueue = Gtk::ListStore::create(this->modelScriptsPriorityQueue); 
}

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

void SatManager::run_script(Glib::ustring scriptName)
{
    try{
        this->scripts[scriptName]->run(this->sat->get_nickname());
    } 
    catch(std::allocator<std::unordered_map<std::string, Script*>> alloc) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the scripts list", __FILE__, __LINE__);
    }
}

void SatManager::enqueue_script(Glib::ustring scriptName)
{
    try {
        this->scriptsQueue.push_back(this->scripts[scriptName]);
    }
    catch(std::allocator<std::unordered_map<std::string, Script*>> alloc) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the scripts list", __FILE__, __LINE__);
    }
}

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

bool SatManager::exists_script(Glib::ustring name)
{
    if(this->scripts.find(name.c_str()) != this->scripts.end())
        return true;
    else
        return false;
}

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

std::stringstream *SatManager::get_first_alias()
{
    return this->scripts.begin()->second->get_alias();
}

std::stringstream *SatManager::get_alias(Glib::ustring scriptName)
{
    return this->scripts[scriptName.c_str()]->get_alias();
}


Glib::RefPtr<Gtk::ListStore> *SatManager::get_model_alias_list(Glib::ustring scriptName)
{
    std::unordered_map<std::string, Script*>::iterator it = this->scripts.find(scriptName.c_str());

    if(it != this->scripts.end())
        return it->second->get_model_alias_list();
    else
        Log::LogWarn(LEVEL_LOG_ERROR, "Script could not be found in the database", __FILE__, __LINE__);

    return NULL;
}

Glib::RefPtr<Gtk::TextBuffer> *SatManager::get_text_buffer(Glib::ustring scriptName)
{
    std::unordered_map<std::string, Script*>::iterator it = this->scripts.find(scriptName.c_str());

    if(it != this->scripts.end())
        return it->second->get_text_buffer();
    else
        Log::LogWarn(LEVEL_LOG_ERROR, "TextBuffer could not be found in the database", __FILE__, __LINE__);

    return NULL;
}

Gtk::TreeModel::RowReference *SatManager::get_row_reference()
{
    return this->row;
}

Glib::RefPtr<Gtk::ListStore>* SatManager::get_scripts_priority_queue()
{
    return &(this->scriptsPriorityQueue);
}


void print_priority_queue(std::vector<Script*> q)
{
    int count = 0;

    for(Script* it : q) {
        std::cout << "scriptQueue[" << count << "] = " << it->get_name()->c_str() << std::endl;
        count++;
    }
}

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

    print_priority_queue(this->scriptsQueue);
}

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

    print_priority_queue(this->scriptsQueue);
}

/* Sets the pointer of fs to a string str in the file */
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

std::stringstream *SatManager::get_save_str(Glib::ustring sessionFile) 
{
    std::fstream fs(sessionFile.c_str(), std::fstream::out);
    std::stringstream *sst = new std::stringstream();
    Glib::ustring name;
    std::vector<Script*> queue;

    if(fs.is_open()) {
        set_stream_ptr(&fs, *this->sat->get_nickname());
        queue = scriptsQueue;

        *sst << "\n" << this->scripts.size() << " ";
        for(unsigned int i=0; i < this->scriptsQueue.size(); i++) {
            queue[i]->save();
            name = *queue[i]->get_name();
            *sst << name << " ";
            name.append(".alias");
            *sst << name << " ";
       }
    }
    fs.close();

    return sst;
}

void SatManager::replace_alias_column_alias(Glib::ustring scriptName, const Glib::ustring& path, const Glib::ustring& newAlias)
{
    if(exists_script(scriptName))
        this->scripts[scriptName.c_str()]->replace_alias_column_alias(path, newAlias);
}

void SatManager::replace_alias_column_command(Glib::ustring scriptName, const Glib::ustring& path, const Glib::ustring& newAlias)
{
    if(exists_script(scriptName))
        this->scripts[scriptName.c_str()]->replace_alias_column_command(path, newAlias);
}
