/*
 * CLASS INIT : loads the data from the satellites
 * and starts XCubeSat_Controller
 *
 */

#ifndef INIT_HPP
#define INIT_HPP

#include <string>

#include "manager.hpp"
#include "data_base.hpp"

#define DEFAULT_OUTPUT              "/tmp/XCubeSat_Controller_Terminal"
#define DEFAULT_SESSION_FILE        "defaults/XCubeSat_Controler.session"
#define DEFAULT_GPREDICT_SATS_FILE  "/tmp/GpredictSATS"

class Init
{
    protected:
        static Manager *LoadCurrentSats(std::string *gpredictSatsFile);
        static Manager *LoadPreviusSection(std::string *previusSection, Manager *man);
    public:
        static Manager *XCubeSat_Controler_Start();
};

#endif
