/*
 * CLASS INIT : loads the data from the satellites
 * and starts XCubeSat_Controller
 *
 */

#ifndef INIT_HPP
#define INIT_HPP

#include <gtkmm.h>

#include "manager.hpp"
#include "data_base.hpp"

#define DEBUG 1
#define DEFAULT_OUTPUT              "/dev/tty1"
#define DEFAULT_SESSION_FILE        "defaults/XCubeSat_Controler.session"
#define DEFAULT_GPREDICT_SATS_FILE  "/tmp/GpredictSATS"

class Init
{
    private:
        InOutInterface *inOutInterface;

    protected:
        Init();
        Manager *LoadCurrentSats(Glib::ustring *gpredictSatsFile);
        Manager *LoadPreviusSection(Glib::ustring *previusSection, Manager *man);
    public:
        static void XCubeSat_Controler_Start(Manager **man, InOutInterface **inter);
};

#endif
