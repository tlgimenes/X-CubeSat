/*
 * CLASS DATA_BASE : this class is an interface between 
 * the program and the data base
 */

#include <fstream>
#include <sstream>

#include "data_base.hpp"

std::stringstream *DataBase::GetSection(Glib::ustring key)
{
    std::ifstream ifs(key.c_str());

    Glib::ustring * content = new Glib::ustring((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    return new std::stringstream(*content);
}

std::stringstream *DataBase::GetScripts(Glib::ustring key)
{
    return DataBase::GetSection(key);
}

std::stringstream *DataBase::GetAlias(Glib::ustring key)
{
    return DataBase::GetSection(key);
}

std::stringstream *DataBase::GetSats(Glib::ustring key)
{
    return DataBase::GetSection(key);
}

bool DataBase::existsSection(Glib::ustring key)
{
    std::ifstream ifit(key.c_str(), std::ifstream::in);

    if(ifit.good()) 
        return true;
    else
        return false;
}

bool DataBase::existsScript(Glib::ustring key)
{
    return DataBase::existsSection(key);
}

bool DataBase::existsAlias(Glib::ustring key)
{
    return DataBase::existsSection(key);
}

bool DataBase::existsSats(Glib::ustring key)
{
    return DataBase::existsSection(key);
}
