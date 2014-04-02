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
        static void load_previous_session(Glib::ustring *gpredictSats, Manager *man, InOutInterface *inter);
        static void load_scripts(int scriptsNum, std::stringstream *scripts, Glib::ustring *satName, Manager *man, InOutInterface *inter);
    public:
        static void XCubeSat_Controller_start(Manager **man, InOutInterface **inter);
};

#endif
