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

#ifndef MODEM_DEFAULT_HPP
#define MODEM_DEFAULT_HPP

#include <vector>
#include <string>
#include <gtkmm.h>

#define CTRL_C '\3'

#define DEFAULT_EXIT_NONE_MODE   ""
#define DEFAULT_EXIT_ASCII_MODE  ""
#define DEFAULT_EXIT_CONFIG_MODE ""

typedef enum modes_t
{
    NONE,
    ASCII,
    CONFIG
} modes_t;

class ModemDefault
{
     protected:
        std::string m_name;
        std::string m_OEM;
        std::string m_REPLY_OEM;
        mode_t m_mode;
        std::vector<std::string> m_modes;

    public:
        ModemDefault(modes_t mode);
        std::vector<std::string> modes();
        std::string name();

        std::string REPLY_OEM();

        virtual Glib::ustring change_modem_mode(Glib::ustring mode);
        virtual Glib::ustring format_send(Glib::ustring str);
};

/*  DO NOT CHANGE THE ORDER ! */
static std::vector<modes_t>     __mmod = { NONE ,  ASCII ,  CONFIG};
static std::vector<std::string> __mstr = {"NONE", "ASCII", "CONFIG"};

#endif /* MODEM_DEFAULT_HPP */
