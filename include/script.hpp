/*
 * CLASS SCRIPT : Each script have an interpreter 
 * which cans run the script.
 *
 */

#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "in_out_log.hpp"
#include "interpreter.hpp"

class Script
{
    private:
        std::string *name;
        std::string *script;
        std::unordered_map<std::string, std::string> *alias;
        std::vector<InOutLog*> logList;

        Interpreter *interpreter;

    public:
        Script(std::string *name, std::string *script, std::string *aliasList, Interpreter *inter);
        std::string *GetName();
        void Run();
};

#endif
