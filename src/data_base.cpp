/*
 * CLASS DATA_BASE : this class is an interface between 
 * the program and the data base
 */
#include <sqlite3.h>
#include <libsqlitewrapped.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include "data_base.hpp"
#include "log.hpp"
#include "defs.hpp"

#define CHECK_CONNECTED(db) \
    if (!db.Connected()) { \
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to connect to the DATA BASE to set a sci/wod data", __FILE__, __LINE__);}
void DataBase::init()
{ 
    Database db(DATABASE_NAME);
    Query q(db);
    CHECK_CONNECTED(q);

    std::stringstream sstr;

    sstr << ".read " << DATABASE_CONFIG;
    q.execute(sstr.str().c_str());
}

std::stringstream *DataBase::get_section(Glib::ustring key)
{
    std::ifstream ifs(key.c_str());

    Glib::ustring * content = new Glib::ustring((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    return new std::stringstream(*content);
}

std::stringstream *DataBase::get_scripts(Glib::ustring key)
{
    Database db(DATABASE_NAME);
    CHECK_CONNECTED(db);

    Query q(db);
    CHECK_CONNECTED(q);

    std::stringstream sstr;

    sstr << "select ScriptContent from " << SCRIPTS_DATA_TABLE << " where " << "ScriptName='" << key << "'";
    std::stringstream *scriptContent = new std::stringstream(q.get_string(sstr.str().c_str()));

    return scriptContent;
}

std::stringstream *DataBase::get_alias(Glib::ustring key)
{
    Database db(DATABASE_NAME);
    CHECK_CONNECTED(db);

    Query q(db);
    CHECK_CONNECTED(q);

    std::stringstream sstr;

    sstr << "select Alias from " << SCRIPTS_DATA_TABLE << " where " << "ScriptName='" << key << "'";
    std::stringstream *alias = new std::stringstream(q.get_string(sstr.str().c_str()));

    return alias;
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
    Database db(DATABASE_NAME);
    CHECK_CONNECTED(db);

    Query q(db);
    CHECK_CONNECTED(q);

    std::stringstream sstr;

    sstr << "select Alias from " << SCRIPTS_DATA_TABLE << " where " << "ScriptName='" << key << "'";
    if(!q.get_count(sstr.str().c_str()))
        return false;
    return true;
}

bool DataBase::exists_alias(Glib::ustring key)
{
    Database db(DATABASE_NAME);
    CHECK_CONNECTED(db);

    Query q(db);
    CHECK_CONNECTED(q);

    std::stringstream sstr;

    sstr << "select Alias from " << SCRIPTS_DATA_TABLE << " where " << "ScriptName='" << key << "'";
    if(!q.get_count(sstr.str().c_str()))
        return false;
    return true;
}

bool DataBase::exists_sats(Glib::ustring key)
{
    return DataBase::exists_section(key);
}

/* TODO: See if it is working */
bool DataBase::save_sci_data(sci_data data)
{
    Database db(DATABASE_NAME);
    CHECK_CONNECTED(db);

    Query q(db);
    CHECK_CONNECTED(q);

    std::stringstream sstr;

    sstr << "insert into " << SCI_DATA_TABLE << " values('" << *(data.satName) << "," << std::to_string(data.timeStamp) << "," << *(data.rawContent) << "," << *(data.dataNickname) << "')";
    q.execute(sstr.str().c_str());

    if(q.GetErrno()) {
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to connect to the DATA BASE to set a sci/wod data", __FILE__, __LINE__);
        return false;
    }
    return true;
}

bool DataBase::save_wod_data(wod_data data)
{
    Database db(DATABASE_NAME);
    CHECK_CONNECTED(db);

    Query q(db);
    CHECK_CONNECTED(q);

    std::stringstream sstr;

    sstr << "insert into " << WOD_DATA_TABLE << "(SatName) values('" << *(data.satName) << "," << std::to_string(data.timeStamp) << "," << *(data.rawContent) <<"')";
    q.execute(sstr.str().c_str());

    if(q.GetErrno()){
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to connect to the DATA BASE to set a sci/wod data", __FILE__, __LINE__);
        return false;
    }
    return true;
}

Glib::ustring DataBase::get_content_file(Glib::ustring fileName)
{
    std::stringstream sstr;
    Glib::ustring data;
    std::fstream file(fileName.c_str(), std::fstream::in);

    if(file.is_open()) {
std::cout << "Openning file" << std::endl;
        sstr << file;
        data = sstr.str();
    }
    return data;
}

/* TODO: end this */
bool DataBase::save_script(Glib::ustring scriptName, Glib::ustring scriptContent, Glib::ustring scriptAlias)
{
    Database db(DATABASE_NAME);
    CHECK_CONNECTED(db);

    Query q(db);
    CHECK_CONNECTED(q);

    std::stringstream sstr;

    sstr << "update " << SCRIPTS_DATA_TABLE << " set ScriptContent='" << scriptContent << "' set Alias='" << scriptAlias << "' where ScriptName='" << scriptName <<"'";
    if(q.execute(sstr.str().c_str()))
        return true;
    else {
        sstr << "insert into " << SCRIPTS_DATA_TABLE << "values('" << scriptContent << "," << scriptAlias << "," << scriptName <<"')";
        if(q.execute(sstr.str().c_str()))
            return true;
    }
    return false;
}
