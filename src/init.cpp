/*
 * CLASS INIT : loads the data from the satellites
 * and starts XCubeSat_Controller
 *
 */

#include <sstream>
#include <string>
#include <boost/format.hpp>

#include "init.hpp"
#include "log.hpp"
#include "xcubesat_interpreter.hpp"

Init::Init()
{
    this->inOutInterface = new InOutInterface(new Glib::ustring(DEFAULT_OUTPUT));
}

#define discard_spaces(sstm) \
    while(sstm->peek() == ' ') { \
        sstm->seekg((int)sstm->tellg() + 1); \
    }

Manager *Init::load_previus_section(Glib::ustring *previusSection, Manager* man)
{
    std::stringstream satName;
    Glib::ustring       *satNameStr;
    Glib::ustring       *scriptNameStr;
    Glib::ustring       *aliasNameStr;
    Glib::ustring       *aliasStr;
    Glib::ustring       *scriptStr;
    std::stringstream scriptsNumStr;
    int               scriptsNum;
    char aux[999];

    if(!DataBase::exists_section(*previusSection)) {
        return NULL;
    }

    std::stringstream* psf = DataBase::get_section(*previusSection);
    while(!psf->eof()) {
        psf->getline(aux, 999);
        satNameStr = new Glib::ustring(aux);
        discard_spaces(psf);
        psf->get(aux, 999, ' ');
        sscanf(aux, " %d ", &scriptsNum);
        discard_spaces(psf);

        if(!man->exists_sat(satNameStr)){ // if there is no sat, go to the next one
            psf->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            for(int i=0; i < scriptsNum; i++) {
                psf->get(aux, 999, ' ');
                scriptNameStr = new Glib::ustring(aux);
                discard_spaces(psf);
                psf->getline(aux, 999, ' ');
                aliasNameStr = new Glib::ustring(aux);

                if(DataBase::exists_script(*scriptNameStr) && DataBase::exists_alias(*aliasNameStr)) {
                    // Get the content of the scripts
                    scriptStr = new Glib::ustring(DataBase::get_scripts(scriptNameStr->c_str())->str());
                    aliasStr = new Glib::ustring(DataBase::get_alias(aliasNameStr->c_str())->str());

                    // Creates a new script associated with the satellite
                    man->add_script(satNameStr, scriptNameStr, scriptStr, aliasStr, new XCubeSatInterpreter(this->inOutInterface));
                }
            }
        }
        psf->get();
    }

    return man;
}

Manager *Init::load_current_sats(Glib::ustring * gpredictSats)
{
    if(!DataBase::exists_sats(*gpredictSats)) {
        Log::LogWarn(LEVEL_LOG_ERROR, "Could not open Sats file from Gpredict !", __FILE__, __LINE__);
    }
    std::stringstream *sats = DataBase::get_sats(*gpredictSats);
    Glib::ustring *satNameStr;
    char aux[999];
    
    Manager * man = new Manager();

    while(!sats->eof()) {
        sats->getline(aux, 999);
        satNameStr = new Glib::ustring(aux);
        man->add_sat(satNameStr);
        sats->peek();
    }

    return man;
}

void Init::XCubeSat_Controller_start(Manager **man, InOutInterface **inter)
{
    Init *init = new Init();

    *man = init->load_current_sats(new Glib::ustring(DEFAULT_GPREDICT_SATS_FILE));
    *man = init->load_previus_section(new Glib::ustring(DEFAULT_SESSION_FILE), *man);

    *inter = init->inOutInterface;

    return;
}

