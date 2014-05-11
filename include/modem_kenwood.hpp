/* X-CubeSat Controller: Real-time communication with satellite program

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

#ifndef MODEM_KENWOOD_HPP
#define MODEM_KENWOOD_HPP

#include "modem_default.hpp"

#define KENWOOD_EXIT_NONE_MODE   "" 
#define KENWOOD_EXIT_ASCII_MODE  ""
#define KENWOOD_EXIT_CONFIG_MODE ""

class ModemKenwood : public ModemDefault
{
    public:
        ModemKenwood(modes_t mode);

        Glib::ustring change_modem_mode(Glib::ustring mode);
        
        Glib::ustring format_send(Glib::ustring str);
        //Glib::ustring change_modem_config_mode();
};

#endif /* MODEM_KENWOOD_HPP */
