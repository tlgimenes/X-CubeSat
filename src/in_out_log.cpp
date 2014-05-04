/*
 * CLASS IN_OUT_LOG : A log of the inout/output operation.
 *
 */
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

#include <ctime>

#include "in_out_log.hpp"
#include "log.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
InOutLog::InOutLog(log_nature nature, Glib::ustring *id, Glib::ustring *data, bool success)
{
    this->nature = nature;
    this->id = id;
    this->data = data;
    this->success = success;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor
 */
InOutLog::InOutLog(log_nature nature, Glib::ustring *id, Glib::ustring *data)
{
    this->nature = nature;
    this->id = id;
    this->data = data;
    this->success = true;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor
 */
InOutLog::InOutLog(log_nature nature, Glib::ustring *id)
{
    this->nature = nature;
    this->id = id;
    this->data = NULL;
    this->success = true;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor
 */
InOutLog::InOutLog(log_nature nature, Glib::ustring *data, bool success)
{
    time_t raw_time;

    // Sets the id as UTC time
    time(&raw_time);
    this->id = new Glib::ustring(asctime(gmtime(&raw_time)));

    this->nature = nature;
    this->data = data;
    this->success = success;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the nature of the log (SEND_LOG or RECEIVE_LOG)
 */
log_nature InOutLog::get_nature()
{
    return this->nature;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the id of the log
 */
Glib::ustring * InOutLog::get_id()
{
    return this->id;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the data of this log
 */
Glib::ustring * InOutLog::get_data()
{
    return this->data;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns true if it was sucessful, false otherwise
 */
bool InOutLog::is_successful()
{
    return this->success;
}
/*  --------------------------------------------------------  */
