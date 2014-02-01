/*
 * CLASS SAT_MANAGER : Each manager has one satellite
 * where it cans send and receive data.
 *
 */

#ifndef SAT_MANAGER_HPP
#define SAT_MANAGER_HPP

#include <unordered_map>
#include <queue>
#include <string>

#include "satellite.hpp"
#include "script.hpp"
#include "interpreter.hpp"

class SatManager
{
    private:
        Satellite * sat;

        std::unordered_map<std::string, Script*> scripts;
        std::queue<Script*> scriptsQueue;

    public:
        SatManager(Satellite *sat);

        void AddScript(std::string *name, std::string *script, std::string *aliasList, Interpreter *inter);
        void RunScript(std::string scriptName);

        void EnqueueScript(std::string scriptName);
        void RunNextScript();
};

#endif
