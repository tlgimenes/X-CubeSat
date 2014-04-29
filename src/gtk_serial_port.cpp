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

#include <gtkmm.h>

#include "gtk_serial_port.hpp"
#include "serial_port.hpp"
#include "log.hpp"

/*  --------------------------------------------------------  */
/* Implementation details of GtkSerialPort class. 
 */
class GtkSerialPortImpl
{
    public:
        CallbackSerialPort serial;
        Glib::ustring receivedData;
};
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
GtkSerialPort::GtkSerialPort(): pimpl(new GtkSerialPortImpl)
{
}

    GtkSerialPort::GtkSerialPort(Glib::ustring devname, unsigned int baudrate)
: pimpl(new GtkSerialPortImpl)
{
    open(devname,baudrate);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void GtkSerialPort::open(Glib::ustring devname, unsigned int baudrate)
{
    try {
        pimpl->serial.open(devname, baudrate);
    } catch(boost::system::system_error&)
    {
        Log::LogWarn(LEVEL_LOG_WARNING, "Unable to open port", __FILE__, __LINE__);
    }
    pimpl->serial.set_callback(bind(&GtkSerialPort::read_callback,this, _1, _2));
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void GtkSerialPort::close()
{
    pimpl->serial.clear_callback();
    try {
        pimpl->serial.close();
    } catch(boost::system::system_error&)
    {
        //Errors during port close
    }
    pimpl->receivedData.clear();//Clear eventual data remaining in read buffer
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool GtkSerialPort::is_open()
{
    return pimpl->serial.is_open();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool GtkSerialPort::error_status()
{
    return pimpl->serial.error_status();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void GtkSerialPort::write(Glib::ustring data)
{
    pimpl->serial.write(data);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
GtkSerialPort::~GtkSerialPort()
{
    pimpl->serial.clear_callback();
    try {
        pimpl->serial.close();
    } catch(...)
    {
        //Don't throw from a destructor
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void GtkSerialPort::read_callback(const char *data, size_t size)
{
    pimpl->receivedData+=Glib::ustring(data);

    this->m_signal_data_received(Glib::ustring(data));
    //if(pimpl->receivedData.find("\n", 0))
    //{
      //  Glib::ustringList lineList=pimpl->receivedData.split(QRegExp("\r\n|\n"));
        //If line ends with \n lineList will contain a trailing empty string
        //otherwise it will contain part of a line without the terminating \n
        //In both cases lineList.at(lineList.size()-1) should not be sent
        //with emit.
        //int numLines=lineList.size()-1;
        //pimpl->receivedData=lineList.at(lineList.size()-1);
        //for(int i=0;i<numLines;i++)
        //{
            //emit line_received(lineList.at(i));
        //}
    //}
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/*  Returns the signal */
sigc::signal<void, Glib::ustring> GtkSerialPort::signal_data_received()
{
    return this->m_signal_data_received;
}
