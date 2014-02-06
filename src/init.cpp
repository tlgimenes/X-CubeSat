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

Init::Init()
{
    this->inOutInterface = new InOutInterface(new Glib::ustring(DEFAULT_OUTPUT));
}

#define discard_spaces(sstm) \
    while(sstm->peek() == ' ') { \
        sstm->seekg((int)sstm->tellg() + 1); \
    }

Manager *Init::LoadPreviusSection(Glib::ustring *previusSection, Manager* man)
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

    if(!DataBase::existsSection(*previusSection)) {
        return NULL;
    }

    std::stringstream* psf = DataBase::GetSection(*previusSection);
    while(!psf->eof()) {
        psf->getline(aux, 999);
        satNameStr = new Glib::ustring(aux);
        discard_spaces(psf);
        psf->get(aux, 999, ' ');
        sscanf(aux, " %d ", &scriptsNum);
        discard_spaces(psf);

        if(!man->existsSat(satNameStr)){ // if there is no sat, go to the next one
            psf->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            for(int i=1; i < scriptsNum; i++) {
                psf->get(aux, 999, ' ');
                scriptNameStr = new Glib::ustring(aux);
                discard_spaces(psf);
                psf->getline(aux, 999, ' ');
                aliasNameStr = new Glib::ustring(aux);

                if(DataBase::existsScript(*scriptNameStr) && DataBase::existsAlias(*aliasNameStr)) {
                    // Get the content of the scripts
                    scriptStr = new Glib::ustring(DataBase::GetScripts(scriptNameStr->c_str())->str());
                    aliasStr = new Glib::ustring(DataBase::GetAlias(aliasNameStr->c_str())->str());

                    // Creates a new script associated with the satellite
                    man->AddScript(satNameStr, scriptNameStr, scriptStr, aliasStr, new Interpreter(this->inOutInterface));
                }
            }
            psf->get(aux, 999, ' ');
            scriptNameStr = new Glib::ustring(aux);
            discard_spaces(psf);
            psf->getline(aux, 999);
            aliasNameStr = new Glib::ustring(aux);

            if(DataBase::existsScript(*scriptNameStr) && DataBase::existsAlias(*aliasNameStr)) {
                // Get the content of the scripts
                scriptStr = new Glib::ustring(DataBase::GetScripts(scriptNameStr->c_str())->str());
                aliasStr = new Glib::ustring(DataBase::GetAlias(aliasNameStr->c_str())->str());

                // Creates a new script associated with the satellite
                man->AddScript(satNameStr, scriptNameStr, scriptStr, aliasStr, new Interpreter(this->inOutInterface));
            }
        }
        psf->peek();
    }

    return man;
}

Manager *Init::LoadCurrentSats(Glib::ustring * gpredictSats)
{
    if(!DataBase::existsSats(*gpredictSats)) {
        Log::LogWarn(LEVEL_LOG_ERROR, "Could not open Sats file from Gpredict !", __FILE__, __LINE__);
    }
    std::stringstream *sats = DataBase::GetSats(*gpredictSats);
    Glib::ustring *satNameStr;
    char aux[999];
    
    Manager * man = new Manager();

    while(!sats->eof()) {
        sats->getline(aux, 999);
        satNameStr = new Glib::ustring(aux);
        man->AddSat(satNameStr);
        sats->peek();
    }

    return man;
}

void Init::XCubeSat_Controler_Start(Manager **man, InOutInterface **inter)
{
    Init *init = new Init();

    *man = init->LoadCurrentSats(new Glib::ustring(DEFAULT_GPREDICT_SATS_FILE));
    *man = init->LoadPreviusSection(new Glib::ustring(DEFAULT_SESSION_FILE), *man);

    *inter = init->inOutInterface;

    return;
}

