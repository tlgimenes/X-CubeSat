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

#ifndef GTK_SERIAL_PORT_HPP
#define GTK_SERIAL_PORT_HPP

#include <gtkmm.h>
#include <boost/shared_ptr.hpp>

class GtkSerialPortImpl;

/**
 * Asynchronous serial class that integrates well with Qt GUI applictions.
 */
class GtkSerialPort //: public QObject
{
    //    Q_OBJECT

    public:
        /**
         * Default constructor
         */
        GtkSerialPort();

        /**
         * Constructor. Opens a serial port
         * \param devname port name, like "/dev/ttyUSB0" or "COM4"
         * \param baudrate port baud rate, example 115200
         * Format is 8N1, flow control is disabled.
         */
        GtkSerialPort(Glib::ustring devname, unsigned int baudrate);

        /**
         * Opens a serial port
         * \param devname port name, like "/dev/ttyUSB0" or "COM4"
         * \param baudrate port baud rate, example 115200
         * Format is 8N1, flow control is disabled.
         */
        void open(Glib::ustring devname, unsigned int baudrate);

        /**
         * Closes a serial port.
         */
        void close();

        /**
         * \return true if the port is open
         */
        bool is_open();

        /**
         * \return true if any error
         */
        bool error_status();

        /**
         * Write a string to the serial port
         */
        void write(Glib::ustring data);

        /**
         * Destructor
         */
        ~GtkSerialPort();

        //signals:

        /**
         * Signal called when data is received from the serial port.
         * This signal is line based, data is grouped by line and a signal
         * is emitted for each line.
         * \param data the line of text just received.
         */
         sigc::signal<void, Glib::ustring> signal_data_received;

    private:
        /**
         * Called when data is received
         */
        void read_callback(const char *data, size_t size);

        boost::shared_ptr<GtkSerialPortImpl> pimpl; ///< Pimpl idiom
};

#endif /* GTK_SERIAL_PORT_HPP */
