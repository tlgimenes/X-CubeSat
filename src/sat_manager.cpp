/*
 * CLASS SAT_MANAGER : Each manager has one satellite
 * where it cans send and receive data.
 *
 */

#include "sat_manager.hpp"
#include "script.hpp"
#include "log.hpp"

SatManager::SatManager(Satellite *sat)
{
    this->sat = sat;
}

void SatManager::AddScript(std::string *name, std::string *script, std::string *aliasList, Interpreter *inter)
{
    try{
        scripts.insert(std::make_pair<std::string, Script*>(name->c_str(), new Script(name, script, aliasList, inter)));
    }
    catch(std::allocator<std::unordered_map<std::string, Script*>> alloc) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the scripts list", __FILE__, __LINE__);
    }
}

void SatManager::RunScript(std::string scriptName)
{
    try{
        this->scripts[scriptName]->Run();
    } 
    catch(std::allocator<std::unordered_map<std::string, Script*>> alloc) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the scripts list", __FILE__, __LINE__);
    }
}

void SatManager::EnqueueScript(std::string scriptName)
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
