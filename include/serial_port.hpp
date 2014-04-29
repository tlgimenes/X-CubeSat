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

#ifndef SERIAL_PORT_HPP
#define SERIAL_PORT_HPP

#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>
#include <boost/function.hpp>
#include <boost/shared_array.hpp>

/**
 * Used internally (pimpl)
 */
class SerialPortImpl;

/**
 * Asyncronous serial class.
 * Intended to be a base class.
 */
class SerialPort: private boost::noncopyable
{
    public:
        SerialPort();

        /**
         * Constructor. Creates and opens a serial device.
         * \param devname serial device name, example "/dev/ttyS0" or "COM1"
         * \param baud_rate serial baud rate
         * \param opt_parity serial parity, default none
         * \param opt_csize serial character size, default 8bit
         * \param opt_flow serial flow control, default none
         * \param opt_stop serial stop bits, default 1
         * \throws boost::system::system_error if cannot open the
         * serial device
         */
        SerialPort(const std::string& devname, unsigned int baud_rate,
                boost::asio::serial_port_base::parity opt_parity=
                boost::asio::serial_port_base::parity(
                    boost::asio::serial_port_base::parity::none),
                boost::asio::serial_port_base::character_size opt_csize=
                boost::asio::serial_port_base::character_size(8),
                boost::asio::serial_port_base::flow_control opt_flow=
                boost::asio::serial_port_base::flow_control(
                    boost::asio::serial_port_base::flow_control::none),
                boost::asio::serial_port_base::stop_bits opt_stop=
                boost::asio::serial_port_base::stop_bits(
                    boost::asio::serial_port_base::stop_bits::one));

        /**
         * Opens a serial device.
         * \param devname serial device name, example "/dev/ttyS0" or "COM1"
         * \param baud_rate serial baud rate
         * \param opt_parity serial parity, default none
         * \param opt_csize serial character size, default 8bit
         * \param opt_flow serial flow control, default none
         * \param opt_stop serial stop bits, default 1
         * \throws boost::system::system_error if cannot open the
         * serial device
         */
        void open(const std::string& devname, unsigned int baud_rate,
                boost::asio::serial_port_base::parity opt_parity=
                boost::asio::serial_port_base::parity(
                    boost::asio::serial_port_base::parity::none),
                boost::asio::serial_port_base::character_size opt_csize=
                boost::asio::serial_port_base::character_size(8),
                boost::asio::serial_port_base::flow_control opt_flow=
                boost::asio::serial_port_base::flow_control(
                    boost::asio::serial_port_base::flow_control::none),
                boost::asio::serial_port_base::stop_bits opt_stop=
                boost::asio::serial_port_base::stop_bits(
                    boost::asio::serial_port_base::stop_bits::one));

        /**
         * \return true if serial device is open
         */
        bool is_open() const;

        /**
         * \return true if error were found
         */
        bool error_status() const;

        /**
         * Close the serial device
         * \throws boost::system::system_error if any error
         */
        void close();

        /**
         * Write data asynchronously. Returns immediately.
         * \param data array of char to be sent through the serial device
         * \param size array size
         */
        void write(const char *data, size_t size);

        /**
         * Write data asynchronously. Returns immediately.
         * \param data to be sent through the serial device
         */
        void write(const std::vector<char>& data);

        /**
         * Write a string asynchronously. Returns immediately.
         * Can be used to send ASCII data to the serial device.
         * To send binary data, use write()
         * \param s string to send
         */
        void write(const std::string& s);

        virtual ~SerialPort()=0;

        /**
         * Read buffer maximum size
         */
        static const int readBufferSize=512;
    private:

        /**
         * Callback called to start an asynchronous read operation.
         * This callback is called by the io_service in the spawned thread.
         */
        void do_read();

        /**
         * Callback called at the end of the asynchronous operation.
         * This callback is called by the io_service in the spawned thread.
         */
        void read_end(const boost::system::error_code& error,
                size_t bytes_transferred);

        /**
         * Callback called to start an asynchronous write operation.
         * If it is already in progress, does nothing.
         * This callback is called by the io_service in the spawned thread.
         */
        void do_write();

        /**
         * Callback called at the end of an asynchronuous write operation,
         * if there is more data to write, restarts a new write operation.
         * This callback is called by the io_service in the spawned thread.
         */
        void write_end(const boost::system::error_code& error);

        /**
         * Callback to close serial port
         */
        void do_close();

        boost::shared_ptr<SerialPortImpl> pimpl;

    protected:

        /**
         * To allow derived classes to report errors
         * \param e error status
         */
        void set_error_status(bool e);

        /**
         * To allow derived classes to set a read callback
         */
        void set_read_callback(const
                boost::function<void (const char*, size_t)>& callback);

        /**
         * To unregister the read callback in the derived class destructor so it
         * does not get called after the derived class destructor but before the
         * base class destructor
         */
        void clear_read_callback();

};

/**
 * Asynchronous serial class with read callback. User code can write data
 * from one thread, and read data will be reported through a callback called
 * from a separate thred.
 */
class CallbackSerialPort: public SerialPort
{
    public:
        CallbackSerialPort();

        /**
         * Opens a serial device.
         * \param devname serial device name, example "/dev/ttyS0" or "COM1"
         * \param baud_rate serial baud rate
         * \param opt_parity serial parity, default none
         * \param opt_csize serial character size, default 8bit
         * \param opt_flow serial flow control, default none
         * \param opt_stop serial stop bits, default 1
         * \throws boost::system::system_error if cannot open the
         * serial device
         */
        CallbackSerialPort(const std::string& devname, unsigned int baud_rate,
                boost::asio::serial_port_base::parity opt_parity=
                boost::asio::serial_port_base::parity(
                    boost::asio::serial_port_base::parity::none),
                boost::asio::serial_port_base::character_size opt_csize=
                boost::asio::serial_port_base::character_size(8),
                boost::asio::serial_port_base::flow_control opt_flow=
                boost::asio::serial_port_base::flow_control(
                    boost::asio::serial_port_base::flow_control::none),
                boost::asio::serial_port_base::stop_bits opt_stop=
                boost::asio::serial_port_base::stop_bits(
                    boost::asio::serial_port_base::stop_bits::one));

        /**
         * Set the read callback, the callback will be called from a thread
         * owned by the CallbackSerialPort class when data arrives from the
         * serial port.
         * \param callback the receive callback
         */
        void set_callback(const
                boost::function<void (const char*, size_t)>& callback);

        /**
         * Removes the callback. Any data received after this function call will
         * be lost.
         */
        void clear_callback();

        virtual ~CallbackSerialPort();
};

#endif /* SERIAL_PORT_HPP */
