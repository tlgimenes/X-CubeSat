/*
 * CLASS DATA_BASE : this class is an interface between 
 * the program and the data base
 */

#include <fstream>
#include <sstream>

#include "data_base.hpp"

std::stringstream *DataBase::get_section(Glib::ustring key)
{
    std::ifstream ifs(key.c_str());

    Glib::ustring * content = new Glib::ustring((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    return new std::stringstream(*content);
}

std::stringstream *DataBase::get_scripts(Glib::ustring key)
{
    return DataBase::get_section(key);
}

std::stringstream *DataBase::get_alias(Glib::ustring key)
{
    return DataBase::get_section(key);
}

std::stringstream *DataBase::get_sats(Glib::ustring key)
{
    return DataBase::get_section(key);
}

bool DataBase::exists_section(Glib::ustring key)
{
    std::ifstream ifit(key.c_str(), std::ifstream::in);

    if(ifit.good()) 
        return true;
    else
        return false;
}

bool DataBase::exists_script(Glib::ustring key)
{
    return DataBase::exists_section(key);
}

bool DataBase::exists_alias(Glib::ustring key)
{
    return DataBase::exists_section(key);
}

bool DataBase::exists_sats(Glib::ustring key)
{
    return DataBase::exists_section(key);
}
