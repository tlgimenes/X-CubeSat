/*
 * CLASS SCRIPT : Each script have an interpreter 
 * which cans run the script.
 *
 */

#include  <boost/format.hpp>
#include <sstream>

#include "script.hpp"

Script::Script(std::string* name, std::string *script, std::string *aliasList, Interpreter *inter)
{
    this->name = name;
    this->script = script;
    this->interpreter = inter;

    /* Creates alias hash table */
    this->alias = new std::unordered_map<std::string, std::string>();
    std::stringstream aliasStr;
    std::stringstream commandStr;

    while(aliasList->size() > 0) {
        aliasStr << boost::format(" %s ") % aliasList->c_str();
        commandStr << boost::format(" %s ") % aliasList->c_str();
        this->alias->insert(std::make_pair<std::string, std::string>(aliasStr.str().c_str(), commandStr.str().c_str()));
    }
}

/* rever */
void Script::Run()
{
    this->logList.push_back(this->interpreter->Interpret(script, alias));
}

std::string *Script::GetName()
{
    return this->name;
}
