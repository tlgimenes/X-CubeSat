/*
 * CLASS DATA_BASE : this class is an interface between 
 * the program and the data base
 */

#include <fstream>
#include <sstream>

#include "data_base.hpp"

std::stringstream *DataBase::GetSection(std::string key)
{
    std::ifstream ifs(key.c_str());

    std::string * content = new std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    return new std::stringstream(*content);
}

std::stringstream *DataBase::GetScripts(std::string key)
{
    return DataBase::GetSection(key);
}

std::stringstream *DataBase::GetAlias(std::string key)
{
    return DataBase::GetSection(key);
}

std::stringstream *DataBase::GetSats(std::string key)
{
    return DataBase::GetSection(key);
}

bool DataBase::existsSection(std::string key)
{
    std::ifstream ifit(key.c_str(), std::ifstream::in);

    if(ifit.good()) 
        return true;
    else
        return false;
}

bool DataBase::existsScript(std::string key)
{
    return DataBase::existsSection(key);
}

bool DataBase::existsAlias(std::string key)
{
    return DataBase::existsSection(key);
}

bool DataBase::existsSats(std::string key)
{
    return DataBase::existsSection(key);
}
