/*
 * CLASS IN_OUT_LOG : A log of the inout/output operation.
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

#ifndef IN_OUT_LOG_HPP
#define IN_OUT_LOG_HPP

#include <gtkmm.h>

typedef enum log_nature_t {
    SEND_LOG,
    RECEIVE_LOG,
} log_nature;

class InOutLog
{
    private:
        log_nature  nature;
        Glib::ustring *id;
        bool success;

        Glib::ustring *data;
    public:
        InOutLog(log_nature nature, Glib::ustring *id, Glib::ustring *data, bool success);
        InOutLog(log_nature nature, Glib::ustring *id, Glib::ustring *data);
        InOutLog(log_nature nature, Glib::ustring *id);
        InOutLog(log_nature nature, Glib::ustring *data, bool success);
        log_nature  get_nature();
        Glib::ustring *get_id();
        Glib::ustring *get_data();
        bool is_successful();
};

#endif
