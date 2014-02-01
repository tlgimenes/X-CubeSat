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

Manager *Init::LoadPreviusSection(std::string *previusSection, Manager* man)
{
    std::stringstream satName;
    std::stringstream scriptName;
    std::stringstream aliasName;
    std::string       *satNameStr;
    std::string       *scriptNameStr;
    std::string       *aliasStr;
    std::string       *scriptStr;
    std::stringstream scriptsNumStr;
    int               scriptsNum;

    if(!DataBase::existsSection(*previusSection)) {
        return NULL;
    }

    Manager *manager = new Manager();

    std::stringstream* psf = DataBase::GetSection(*previusSection);
    while(!psf->eof()) {
        satName << boost::format(" %s ") % psf;
        satNameStr = new std::string(satName.str());

        scriptsNumStr << boost::format(" %d ") % psf;
        scriptsNum = std::stoi(scriptsNumStr.str());

        if(!manager->existsSat(satNameStr)){ // if there is no sat go, to the next one
            psf->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            for(int i=0; i < scriptsNum; i++) {
                scriptName << boost::format(" %s ") % psf;
                aliasName  << boost::format(" %s ") % psf;
                if(DataBase::existsScript(scriptName.str()) && DataBase::existsAlias(aliasName.str())) {
                    // Insert the sat

                    // Get the Script name
                    scriptNameStr = new std::string(scriptName.str());

                    // Get the content of the scripts
                    scriptStr = new std::string(DataBase::GetScripts(scriptName.str())->str());
                    aliasStr = new std::string(DataBase::GetAlias(aliasName.str())->str());

                    // Creates a new script associated with the satellite
                    manager->AddScript(satNameStr, scriptNameStr, scriptStr, aliasStr, new Interpreter(new std::string(DEFAULT_OUTPUT)));
                }
            }
        } 
    }

    return manager;
}

Manager *Init::LoadCurrentSats(std::string * gpredictSats)
{
    if(!DataBase::existsSats(*gpredictSats)) {
        Log::LogWarn(LEVEL_LOG_ERROR, "Could not open Sats file from Gpredict !", __FILE__, __LINE__);
    }
    std::stringstream *sats = DataBase::GetSats(*gpredictSats);
    std::stringstream satName;
    std::string *satNameStr;
    
    Manager * man = new Manager();

    while(!sats->eof()) {
        satName << boost::format(" %s\n ") % sats->str();
        satNameStr = new std::string(satName.str());
        man->AddSat(satNameStr);
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
