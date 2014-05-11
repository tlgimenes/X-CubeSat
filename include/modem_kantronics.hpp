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

#ifndef MODEM_KANTRONICS_HPP
#define MODEM_KANTRONICS_HPP

#include <vector>
#include <string>
#include <gtkmm.h>

#include "modem_default.hpp"

#define KANTRONICS_EXIT_ASCII_MODE               "\3X" /* CTRL_C + X */
#define KANTRONICS_EXIT_NONE_MODE                "\3X" /* CTRL_C + X */
#define KANTRONICS_EXIT_CONFIG_MODE              "\3X" /* CTRL_C + X */
#define KANTRONICS_ENTER_TRANSMIT_MODE           "\3T" /* CTRL + T */
#define KANTRONICS_RETURN_RECEIVE_IF_BUFFER_EMPY "\3E" /* CTRL + E */
#define KANTRONICS_RETURN_RECEIVE_IMMEDIATELY    "\3R" /* CTRL_C + R */


class ModemKantronics : public ModemDefault
{
    public:
        ModemKantronics(modes_t mode);

        Glib::ustring change_modem_mode(Glib::ustring mode);
        
        Glib::ustring format_send(Glib::ustring str);
};

#endif /* MODEM_KANTRONICS_HPP */
