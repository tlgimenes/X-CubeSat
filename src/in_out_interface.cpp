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

#include <termios.h>
#include <unistd.h>

#include "in_out_interface.hpp"
#include "init.hpp"
#include "log.hpp"
#include "defs.hpp"
#include "modems.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
InOutInterface::InOutInterface()
{
    termios stored_settings;
    tcgetattr(0, &stored_settings);
    termios new_settings;
    new_settings.c_lflag &= (~ICANON);
    //new_settings.c_lflag &= (~ISIG);
    new_settings.c_lflag &= ~(ECHO);
    tcsetattr(0, TCSANOW, &new_settings);

    //this->port = new CallbackSerialPort(DEFAULT_OUTPUT, DEFAULT_BAUD_RATE);
    this->port = new GtkSerialPort(DEFAULT_OUTPUT, DEFAULT_BAUD_RATE);
    this->deviceName = DEFAULT_OUTPUT;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor
 */
InOutInterface::InOutInterface(Glib::ustring *deviceName)
{
    termios stored_settings;
    tcgetattr(0, &stored_settings);
    termios new_settings;
    new_settings.c_lflag &= (~ICANON);
    //new_settings.c_lflag &= (~ISIG);
    new_settings.c_lflag &= ~(ECHO);
    tcsetattr(0, TCSANOW, &new_settings);

    //this->port = new CallbackSerialPort(deviceName->c_str(), DEFAULT_BAUD_RATE);
    this->port = new GtkSerialPort(deviceName->c_str(), DEFAULT_BAUD_RATE);
    this->deviceName = *deviceName;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/*  Constructor */
InOutInterface::InOutInterface(Glib::ustring *deviceName, int speed)
{
    termios stored_settings;
    tcgetattr(0, &stored_settings);
    termios new_settings;
    new_settings.c_lflag &= (~ICANON);
    //new_settings.c_lflag &= (~ISIG);
    new_settings.c_lflag &= ~(ECHO);
    tcsetattr(0, TCSANOW, &new_settings);

    //this->port = new CallbackSerialPort(deviceName->c_str(), speed);
    this->port = new GtkSerialPort(deviceName->c_str(), speed);
    this->deviceName = *deviceName;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Opens a port named devieName with the speed speed
 */
bool InOutInterface::open(Glib::ustring deviceName, int speed)
{
    if(this->port != NULL) {
        this->port->open(deviceName.c_str(), speed);
        return this->port->is_open();
    }

    return false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Write to the port the string data
 */
InOutLog * InOutInterface::write(Glib::ustring *data)
{
    InOutLog *log = NULL;

    /*  It is here that the '\r' character is added to the
     *  string to be sended for the modem to understand it 
     *  as the end of the line*/
    data->append(OEM);

    if(this->port->is_open()) {
        this->port->write(*data);
        log = new InOutLog(SEND_LOG, NULL, true);
    }
    else {
        log = new InOutLog(SEND_LOG, new Glib::ustring("port not oppenned"), false);
        Log::LogWarn(LEVEL_LOG_WARNING, "Couldn't write because the port was not oppenned", __FILE__, __LINE__);
    }

    return log;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void InOutInterface::set_read_callback(sigc::slot<void, Glib::ustring> slot)
{
    this->port->signal_data_received().connect(slot);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Change the port speed to speed
 */
bool InOutInterface::set_device_speed(int speed)
{
    if(speed >= 0) {
        //return this->port->set_speed(speed);
        Log::LogWarn(LEVEL_LOG_INFO, "Sorry, this feature was not implemented yet!", __FILE__, __LINE__);
        return true;
    }

    return false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Return the name of the port entered
 */
Glib::ustring InOutInterface::get_device_name()
{
    return this->deviceName;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns true if it's oppenned, false otherwise
 */
bool InOutInterface::is_open()
{
    if(this->port != NULL)
        return this->port->is_open();

    return false;
}
/*  --------------------------------------------------------  */
