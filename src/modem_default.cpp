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

#include "modem_default.hpp"
#include "log.hpp"

/* --------------------------------------------------------------*/
ModemDefault::ModemDefault(modes_t mode)
{
    m_name      = "DEFAULT";
    m_mode      = mode;
    m_OEM       = "\r";
    m_REPLY_OEM = "\r\n";

    m_modes.push_back("CONFIG");
    m_modes.push_back("NONE");
}
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
std::vector<std::string> ModemDefault::modes()
{
    return this->m_modes;
}
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
std::string ModemDefault::name()
{
    return this->m_name;
}
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
Glib::ustring ModemDefault::change_modem_mode(Glib::ustring mode)
{
    /* Implemented modes: "NONE", "CONFIG" */

    Glib::ustring str = "";

    /* Get Current status of the modem and generates the 
     * string to get out of it */
    switch(m_mode) 
    {
        case NONE:
        case CONFIG:
            break;
        default:
            Log::LogWarn(LEVEL_LOG_WARNING, "Error in modem mode, please report bug", __FILE__, __LINE__);
    }

    /* Generates the string to get in the next mode of 
     * the modem and change it's mode */
    if (!mode.compare("NONE")) {
        m_mode = NONE;
    }
    else if (!mode.compare("CONFIG")) {
        m_mode = CONFIG;
    }

    return str;
}
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
std::string ModemDefault::REPLY_OEM()
{
    return this->m_REPLY_OEM;
}
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
Glib::ustring ModemDefault::format_send(Glib::ustring str)
{
    Glib::ustring final = "";

    if (this->m_mode == NONE) {
        final.append(str);
        final.append(m_OEM);
    }
    else if (this->m_mode == CONFIG) {
        final.append(str);
        final.append(m_OEM);
    }
    else {
        final.append(str);
        final.append(m_OEM);
    }

    return final;
}
/* --------------------------------------------------------------*/

