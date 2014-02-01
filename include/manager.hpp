/*
 * CLASS MANAGER : This class is the core class of 
 * this program. It gives methods to add some config 
 * files to satellites, enqueue new data to send and 
 * receive data from the satellites.
 *
 */

#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <string>
#include <unordered_map>

#include "sat_manager.hpp"

class Manager
{
    private:
        std::unordered_map<std::string, SatManager*> satManagers;
        std::string *tracking;
        float elevation;

    public:
        Manager();

        void AddSat(std::string *satName);

        void AddScript(std::string *satName, std::string *scriptName, std::string *script, std::string *aliasList, Interpreter *inter);

        bool existsSat(std::string *satName);
};

#endif
