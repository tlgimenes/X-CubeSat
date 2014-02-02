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

#define discard_spaces(sstm) \
    while(sstm->peek() == ' ') { \
        sstm->seekg((int)sstm->tellg() + 1); \
    }

Manager *Init::LoadPreviusSection(std::string *previusSection, Manager* man)
{
    std::stringstream satName;
    std::string       *satNameStr;
    std::string       *scriptNameStr;
    std::string       *aliasNameStr;
    std::string       *aliasStr;
    std::string       *scriptStr;
    std::stringstream scriptsNumStr;
    int               scriptsNum;
    char aux[999];

    if(!DataBase::existsSection(*previusSection)) {
        return NULL;
    }

    std::stringstream* psf = DataBase::GetSection(*previusSection);
    while(!psf->eof()) {
        psf->getline(aux, 999);
        satNameStr = new std::string(aux);
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
                scriptNameStr = new std::string(aux);
                discard_spaces(psf);
                psf->getline(aux, 999, ' ');
                aliasNameStr = new std::string(aux);

                if(DataBase::existsScript(*scriptNameStr) && DataBase::existsAlias(*aliasNameStr)) {
                    // Get the content of the scripts
                    scriptStr = new std::string(DataBase::GetScripts(scriptNameStr->c_str())->str());
                    aliasStr = new std::string(DataBase::GetAlias(aliasNameStr->c_str())->str());

                    // Creates a new script associated with the satellite
                    man->AddScript(satNameStr, scriptNameStr, scriptStr, aliasStr, new Interpreter(new std::string(DEFAULT_OUTPUT)));
                }
            }
            psf->get(aux, 999, ' ');
            scriptNameStr = new std::string(aux);
            discard_spaces(psf);
            psf->getline(aux, 999);
            aliasNameStr = new std::string(aux);

            if(DataBase::existsScript(*scriptNameStr) && DataBase::existsAlias(*aliasNameStr)) {
                // Get the content of the scripts
                scriptStr = new std::string(DataBase::GetScripts(scriptNameStr->c_str())->str());
                aliasStr = new std::string(DataBase::GetAlias(aliasNameStr->c_str())->str());

                // Creates a new script associated with the satellite
                man->AddScript(satNameStr, scriptNameStr, scriptStr, aliasStr, new Interpreter(new std::string(DEFAULT_OUTPUT)));
            }
        }
        psf->peek();
    }

    return man;
}

Manager *Init::LoadCurrentSats(std::string * gpredictSats)
{
    if(!DataBase::existsSats(*gpredictSats)) {
        Log::LogWarn(LEVEL_LOG_ERROR, "Could not open Sats file from Gpredict !", __FILE__, __LINE__);
    }
    std::stringstream *sats = DataBase::GetSats(*gpredictSats);
    std::string *satNameStr;
    char aux[999];
    
    Manager * man = new Manager();

    while(!sats->eof()) {
        sats->getline(aux, 999);
        satNameStr = new std::string(aux);
        man->AddSat(satNameStr);
        sats->peek();
    }

    return man;
}

Manager *Init::XCubeSat_Controler_Start()
{
    Manager * man;
    
    man = Init::LoadCurrentSats(new std::string(DEFAULT_GPREDICT_SATS_FILE));
    man = Init::LoadPreviusSection(new std::string(DEFAULT_SESSION_FILE), man);

    return man;
}