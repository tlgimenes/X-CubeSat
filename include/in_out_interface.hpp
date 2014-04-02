/*
 * CLASS IN_OUT_INTERFACE : This class provides 
 * methods for operating input output for the program.
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

#ifndef IN_OUT_INTERFACE_HPP
#define IN_OUT_INTERFACE_HPP

#include <gtkmm.h>
#include "port.hpp"
#include "in_out_log.hpp"
#include "models.hpp"

class InOutInterface
{
    private:
        Port *port;
        Glib::ustring deviceName;
        
    public:
        InOutInterface();
        InOutInterface(Glib::ustring *portName);

        bool open(Glib::ustring deviceName, int speed);
        InOutLog * write(Glib::ustring *data);
        InOutLog * read(size_t count);
        InOutLog * read(char delim);

        bool set_device_speed(int);

        bool is_oppenned();
        bool is_configured();

        Glib::ustring get_device_name();

 
 //       void SetDeviceName();
 //       void SetDeviceSpeed();

 //       void InitConfigFrameGtk(Gtk::Entry *portName, Gtk::Image *portNameStatus, Gtk::ComboBox *portSpeedComboBox, Gtk::Image *portSpeedStatus);
};

#endif
