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

/*  --------------------------------------------------------  */
#define CHECK_CONNECTED(db) \
    if (!db.Connected()) { \
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to connect to the DATA BASE to set a sci/wod data", __FILE__, __LINE__);}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void DataBase::init()
{ 
    Database db(DATABASE_NAME);
    CHECK_CONNECTED(db);

    Query q(db);
    CHECK_CONNECTED(q);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
int DataBase::get_session_script_num(Glib::ustring satName)
{
    Database db(DATABASE_NAME);
    CHECK_CONNECTED(db);

    Query q(db);
    CHECK_CONNECTED(q);

    std::stringstream sstr;

    sstr << "select AmountOfScripts from " << SESSION_DATA_TABLE << " where " << "SatName='" << satName << "'";
    if(q.execute(sstr.str().c_str())) {
        int num = q.get_count(sstr.str().c_str());

        return num;
    }
    return 0;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
std::stringstream *DataBase::get_session_script_names(Glib::ustring satName)
{ 
    Database db(DATABASE_NAME);
    CHECK_CONNECTED(db);

    Query q(db);
    CHECK_CONNECTED(q);

    std::stringstream sstr;

    sstr << "select ScriptNames from " << SESSION_DATA_TABLE << " where " << "SatName='" << satName << "'";
    if(q.execute(sstr.str().c_str())) {
        std::stringstream *scriptContent = new std::stringstream(q.get_string(sstr.str().c_str()));

        return scriptContent;
    }
    else {
        return NULL;
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
std::stringstream *DataBase::get_section(Glib::ustring key)
{
    std::ifstream ifs(key.c_str());

    Glib::ustring * content = new Glib::ustring((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    return new std::stringstream(*content);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
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
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
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
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
std::stringstream *DataBase::get_sats(Glib::ustring key)
{
    return DataBase::get_section(key);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool DataBase::exists_section(Glib::ustring key)
{
    std::ifstream ifit(key.c_str(), std::ifstream::in);

    if(ifit.good()) 
        return true;
    else
        return false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool DataBase::exists_script(Glib::ustring key)
{
    Database db(DATABASE_NAME);
    CHECK_CONNECTED(db);

    Query q(db);
    CHECK_CONNECTED(q);

    std::stringstream sstr;

    sstr << "select ScriptId from " << SCRIPTS_DATA_TABLE << " where " << "ScriptName='" << key << "'";
    if(!q.get_num(sstr.str().c_str()))
        return false;
    return true;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool DataBase::exists_sats(Glib::ustring key)
{
    Database db(DATABASE_NAME);
    CHECK_CONNECTED(db);

    Query q(db);
    CHECK_CONNECTED(q);

    std::stringstream sstr;

    sstr << "select SatId from " << SESSION_DATA_TABLE << " where " << "SatName='" << key << "'";
    if(!q.get_num(sstr.str().c_str()))
        return false;
    return true;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool DataBase::save_sci_data(sci_data data)
{
    Database db(DATABASE_NAME);
    CHECK_CONNECTED(db);

    Query q(db);
    CHECK_CONNECTED(q);

    std::stringstream sstr;

    sstr << "insert into " << SCI_DATA_TABLE << "(SatName, TimeStamp, RawContent, DataNickName) values('" << *(data.satName) << "," << std::to_string(data.timeStamp) << "," << *(data.rawContent) << "," << *(data.dataNickname) << "')";
    if(!q.execute(sstr.str().c_str())) {
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to connect to the DATA BASE to set a sci/wod data", __FILE__, __LINE__);
        return false;
    }
    return true;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool DataBase::save_wod_data(wod_data data)
{
    Database db(DATABASE_NAME);
    CHECK_CONNECTED(db);

    Query q(db);
    CHECK_CONNECTED(q);

    std::stringstream sstr;

    sstr << "insert into " << WOD_DATA_TABLE << "(SatName, TimeStamp, RawContent, DataNickName) values('" << *(data.satName) << "," << std::to_string(data.timeStamp) << "," << *(data.rawContent) << "," << *(data.dataNickname) << "')";
    if(!q.execute(sstr.str().c_str())) {
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to connect to the DATA BASE to set a sci/wod data", __FILE__, __LINE__);
        return false;
    }
    return true;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
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
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool DataBase::save_script(Glib::ustring scriptName, Glib::ustring scriptContent, Glib::ustring scriptAlias)
{
    Database db(DATABASE_NAME);
    CHECK_CONNECTED(db);

    Query q(db);
    CHECK_CONNECTED(q);

    std::stringstream sstr;
    bool res = false;

    if(DataBase::exists_script(scriptName)) {
        sstr << "update " << SCRIPTS_DATA_TABLE << " set ScriptContent='" << scriptContent << "' where ScriptName='" << scriptName << "'";
        if(q.execute(sstr.str().c_str())) {
            sstr.str("");
            sstr << "update " << SCRIPTS_DATA_TABLE << " set Alias='" << scriptAlias << "' where ScriptName='"<< scriptName << "'";
            res = q.execute(sstr.str().c_str());
        }
    }
    else {
        sstr.str("");
        sstr << "insert into " << SCRIPTS_DATA_TABLE << " (ScriptContent, Alias, ScriptName) values('" << scriptContent << "','" << scriptAlias << "','" << scriptName << "')";
        res = q.execute(sstr.str().c_str());
    }

    if(!res)
        Log::LogWarn(LEVEL_LOG_ERROR, q.GetError().c_str(), __FILE__, __LINE__);

    return res;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool DataBase::save_session(Glib::ustring satName, int numScripts, Glib::ustring scriptNames)
{
    Database db(DATABASE_NAME);
    CHECK_CONNECTED(db);

    Query q(db);
    CHECK_CONNECTED(q);

    std::stringstream sstr;
    bool res = false;

    if(DataBase::exists_sats(satName)) {
        sstr << "update " << SESSION_DATA_TABLE << " set AmountOfScripts='" << numScripts << "' where SatName='" << satName << "'";
        if(q.execute(sstr.str().c_str())) {
            sstr.str("");
            sstr << "update " << SESSION_DATA_TABLE << " set ScriptNames='" << scriptNames << "' where SatName='" << satName << "'";
            res = q.execute(sstr.str().c_str());
        }
    }
    else {
        sstr << "insert into " << SESSION_DATA_TABLE << " (SatName, AmountOfScripts, ScriptNames) values('" << satName << "','" << numScripts << "','" << scriptNames << "')";
        res = q.execute(sstr.str().c_str());
    }

    if(!res) {
        Log::LogWarn(LEVEL_LOG_ERROR, q.GetError().c_str(), __FILE__, __LINE__);
    }

    return res;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool DataBase::exists_file(Glib::ustring file)
{
    std::ifstream f(file);

    if(f)
        return true;
    else
        return false;
}
/*  --------------------------------------------------------  */
