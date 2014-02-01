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
}

void Manager::AddSat(std::string *satName)
{
    try {
        this->satManagers.insert(std::make_pair<std::string, SatManager*>(satName->c_str(), new SatManager(new Satellite(satName))));
    }
    catch(std::allocator<std::unordered_map<std::string, Script*>> alloc) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the satellites list", __FILE__, __LINE__);
    }
}

void Manager::AddScript(std::string *satName, std::string *scriptName, std::string *script, std::string *aliasList, Interpreter *inter)
{   
    try {
        this->satManagers[satName->c_str()]->AddScript(scriptName, script, aliasList, inter);
    }
    catch (std::allocator<std::unordered_map<std::string, SatManager*>> alloc) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Error in the satellites list", __FILE__, __LINE__);
    }
}

bool Manager::existsSat(std::string *satName)
{
    if(this->satManagers.find(*satName) == this->satManagers.end())
        return false;
    else
        return true;
}
