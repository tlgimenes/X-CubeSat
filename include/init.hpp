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
#include "defs.hpp"

class Init
{
    private:
        InOutInterface *inOutInterface;

    protected:
        Init();
        Manager *load_current_sats(Glib::ustring *gpredictSatsFile);
        Manager *load_previus_section(Glib::ustring *previusSection, Manager *man);
    public:
        static void XCubeSat_Controller_start(Manager **man, InOutInterface **inter);
};

#endif
