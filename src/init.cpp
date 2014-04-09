/*
 * CLASS INIT : loads the data from the satellites
 * and starts XCubeSat_Controller
 *
 */
/* X-CubeSat Controler: Real-time communication with satellite program

 Copyright (C)  2014 - Tiago Lobato Gimenes

 Authors: Tiago Lobato Gimenes <tlgimenes@gmail.com>

 Comments, questions and bugreports should be submitted via
 https://github.com/tlgimenes/X-CubeSat
 More details can be found at the project home page:

 https://github.com/tlgimenes/X-CubeSat

 This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

 This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
along with this program; if not, visit http://www.fsf.org/
*/

#include <sstream>
#include <string>
#include <boost/format.hpp>

#include "init.hpp"
#include "log.hpp"
#include "xcubesat_interpreter.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
Init::Init()
{
    this->inOutInterface = new InOutInterface(new Glib::ustring(DEFAULT_OUTPUT));
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Remove ths characters: space from the stringstream sstm
 */
#define discard_spaces(sstm) \
    while(sstm->peek() == ' ') { \
        sstm->seekg((int)sstm->tellg() + 1); \
    }
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Loads the scripts for a given satellite from the database
 * and adds then in the satellite manager.
 */
void Init::load_scripts(int scriptsNum, std::stringstream *scripts, Glib::ustring *satName, Manager *man, InOutInterface *inter)
{
    Glib::ustring *scriptStr, *aliasStr;
    Glib::ustring *scriptNameStr;
    char aux[999];

    for(int i=0; i < scriptsNum && !scripts->eof(); i++) {
        scripts->getline(aux, 999);
        scriptNameStr = new Glib::ustring(aux);
        discard_spaces(scripts);

        if(DataBase::exists_script(*scriptNameStr)) {
            // Get the content of the scripts
            scriptStr = new Glib::ustring(DataBase::get_scripts(scriptNameStr->c_str())->str());
            aliasStr = new Glib::ustring(DataBase::get_alias(scriptNameStr->c_str())->str());

            // Creates a new script associated with the satellite
            man->add_script(satName, scriptNameStr, scriptStr, aliasStr, new XCubeSatInterpreter(inter));
        }
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Loads the satellites and the scripts related to these
 * satellites of previous sessions acording to the 
 * satellites that are being shown by Gpredict
 */
void Init::load_previous_session(Glib::ustring *gpredictSats, Manager *man, InOutInterface *inter)
{
    if(!DataBase::exists_file(*gpredictSats)) {
        Log::LogWarn(LEVEL_LOG_ERROR, "Could not open Sats file from Gpredict !", __FILE__, __LINE__);
   }
    std::stringstream *sats = DataBase::get_sats(*gpredictSats);
    Glib::ustring *satNameStr;
    char aux[999];
    
    sats->getline(aux, 999);
    while(!sats->eof()) {
        satNameStr = new Glib::ustring(aux);
        man->add_sat(satNameStr);
        load_scripts(DataBase::get_session_script_num(*satNameStr), DataBase::get_session_script_names(*satNameStr), satNameStr, man, inter);
        sats->getline(aux, 999);
    }

    return;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Instantiates and Instantiates a new Manager and a new InOutInterface
 */
void Init::XCubeSat_Controller_start(Manager **man, InOutInterface **inter)
{
    *inter = new InOutInterface(new Glib::ustring(DEFAULT_OUTPUT));

    *man = new Manager();
    Init::load_previous_session(new Glib::ustring(DEFAULT_GPREDICT_SATS_FILE), *man, *inter);
}
/*  --------------------------------------------------------  */
