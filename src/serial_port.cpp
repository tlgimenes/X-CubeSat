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

#include "serial_port.hpp"

#include <string>
#include <algorithm>
#include <iostream>
#include <boost/bind.hpp>

//
//Class SerialPort
//

#ifndef __APPLE__

/*  --------------------------------------------------------  */
class SerialPortImpl: private boost::noncopyable
{
    public:
        SerialPortImpl(): io(), port(io), backgroundThread(), open(false),
        error(false) {}

        boost::asio::io_service io; ///< Io service object
        boost::asio::serial_port port; ///< Serial port object
        boost::thread backgroundThread; ///< Thread that runs read/write operations
        bool open; ///< True if port open
        bool error; ///< Error flag
        mutable boost::mutex errorMutex; ///< Mutex for access to error

        /// Data are queued here before they go in writeBuffer
        std::vector<char> writeQueue;
        boost::shared_array<char> writeBuffer; ///< Data being written
        size_t writeBufferSize; ///< Size of writeBuffer
        boost::mutex writeQueueMutex; ///< Mutex for access to writeQueue
        char readBuffer[SerialPort::readBufferSize]; ///< data being read

        /// Read complete callback
        boost::function<void (const char*, size_t)> callback;
};
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
SerialPort::SerialPort(): pimpl(new SerialPortImpl)
{

}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
SerialPort::SerialPort(const std::string& devname, unsigned int baud_rate,
        boost::asio::serial_port_base::parity opt_parity,
        boost::asio::serial_port_base::character_size opt_csize,
        boost::asio::serial_port_base::flow_control opt_flow,
        boost::asio::serial_port_base::stop_bits opt_stop)
: pimpl(new SerialPortImpl)
{
    open(devname,baud_rate,opt_parity,opt_csize,opt_flow,opt_stop);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::open(const std::string& devname, unsigned int baud_rate,
        boost::asio::serial_port_base::parity opt_parity,
        boost::asio::serial_port_base::character_size opt_csize,
        boost::asio::serial_port_base::flow_control opt_flow,
        boost::asio::serial_port_base::stop_bits opt_stop)
{
    if(is_open()) close();

    set_error_status(true);//If an exception is thrown, error_ remains true
    pimpl->port.open(devname);
    pimpl->port.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    pimpl->port.set_option(opt_parity);
    pimpl->port.set_option(opt_csize);
    pimpl->port.set_option(opt_flow);
    pimpl->port.set_option(opt_stop);

    //This gives some work to the io_service before it is started
    pimpl->io.post(boost::bind(&SerialPort::do_read, this));

    boost::thread t(boost::bind(&boost::asio::io_service::run, &pimpl->io));
    pimpl->backgroundThread.swap(t);
    set_error_status(false);//If we get here, no error
    pimpl->open=true; //Port is now open
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool SerialPort::is_open() const
{
    return pimpl->open;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool SerialPort::error_status() const
{
    boost::lock_guard<boost::mutex> l(pimpl->errorMutex);
    return pimpl->error;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::close()
{
    if(!is_open()) return;

    pimpl->open=false;
    pimpl->io.post(boost::bind(&SerialPort::do_close, this));
    pimpl->backgroundThread.join();
    pimpl->io.reset();
    if(error_status())
    {
        throw(boost::system::system_error(boost::system::error_code(),
                    "Error while closing the device"));
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::write(const char *data, size_t size)
{
    {
        boost::lock_guard<boost::mutex> l(pimpl->writeQueueMutex);
        pimpl->writeQueue.insert(pimpl->writeQueue.end(),data,data+size);
    }
    pimpl->io.post(boost::bind(&SerialPort::do_write, this));
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::write(const std::vector<char>& data)
{
    {
        boost::lock_guard<boost::mutex> l(pimpl->writeQueueMutex);
        pimpl->writeQueue.insert(pimpl->writeQueue.end(),data.begin(),
                data.end());
    }
    pimpl->io.post(boost::bind(&SerialPort::do_write, this));
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::write(const std::string& s)
{
    {
        boost::lock_guard<boost::mutex> l(pimpl->writeQueueMutex);
        pimpl->writeQueue.insert(pimpl->writeQueue.end(),s.begin(),s.end());
    }
    pimpl->io.post(boost::bind(&SerialPort::do_write, this));
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
SerialPort::~SerialPort()
{
    if(is_open())
    {
        try {
            close();
        } catch(...)
        {
            //Don't throw from a destructor
        }
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::do_read()
{
    pimpl->port.async_read_some(boost::asio::buffer(pimpl->readBuffer,readBufferSize),
            boost::bind(&SerialPort::readEnd,
                this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::readEnd(const boost::system::error_code& error,
        size_t bytes_transferred)
{
    if(error)
    {
#ifdef __APPLE__
        if(error.value()==45)
        {
            //Bug on OS X, it might be necessary to repeat the setup
            //http://osdir.com/ml/lib.boost.boost::asio.user/2008-08/msg00004.html
            do_read();
            return;
        }
#endif //__APPLE__
        //error can be true even because the serial port was closed.
        //In this case it is not a real error, so ignore
        if(is_open())
        {
            do_close();
            set_error_status(true);
        }
    } else {
        if(pimpl->callback) pimpl->callback(pimpl->readBuffer,
                bytes_transferred);
        do_read();
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::do_write()
{
    //If a write operation is already in progress, do nothing
    if(pimpl->writeBuffer==0)
    {
        boost::lock_guard<boost::mutex> l(pimpl->writeQueueMutex);
        pimpl->writeBufferSize=pimpl->writeQueue.size();
        pimpl->writeBuffer.reset(new char[pimpl->writeQueue.size()]);
        copy(pimpl->writeQueue.begin(),pimpl->writeQueue.end(),
                pimpl->writeBuffer.get());
        pimpl->writeQueue.clear();
        async_write(pimpl->port,boost::asio::buffer(pimpl->writeBuffer.get(),
                    pimpl->writeBufferSize),
                boost::bind(&SerialPort::write_end, this, boost::asio::placeholders::error));
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::write_end(const boost::system::error_code& error)
{
    if(!error)
    {
        boost::lock_guard<boost::mutex> l(pimpl->writeQueueMutex);
        if(pimpl->writeQueue.empty())
        {
            pimpl->writeBuffer.reset();
            pimpl->writeBufferSize=0;

            return;
        }
        pimpl->writeBufferSize=pimpl->writeQueue.size();
        pimpl->writeBuffer.reset(new char[pimpl->writeQueue.size()]);
        copy(pimpl->writeQueue.begin(),pimpl->writeQueue.end(),
                pimpl->writeBuffer.get());
        pimpl->writeQueue.clear();
        async_write(pimpl->port,boost::asio::buffer(pimpl->writeBuffer.get(),
                    pimpl->writeBufferSize),
                boost::bind(&SerialPort::write_end, this, boost::asio::placeholders::error));
    } else {
        set_error_status(true);
        do_close();
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::do_close()
{
    boost::system::error_code ec;
    pimpl->port.cancel(ec);
    if(ec) set_error_status(true);
    pimpl->port.close(ec);
    if(ec) set_error_status(true);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::set_error_status(bool e)
{
    boost::lock_guard<boost::mutex> l(pimpl->errorMutex);
    pimpl->error=e;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::set_read_callback(const boost::function<void (const char*, size_t)>& callback)
{
    pimpl->callback=callback;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::clear_read_callback()
{
    pimpl->callback.clear();
}
/*  --------------------------------------------------------  */

#else //__APPLE__

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

/*  --------------------------------------------------------  */
class SerialPortImpl: private boost::noncopyable
{
    public:
        SerialPortImpl(): backgroundThread(), open(false), error(false) {}

        boost::thread backgroundThread; ///< Thread that runs read operations
        bool open; ///< True if port open
        bool error; ///< Error flag
        mutable boost::mutex errorMutex; ///< Mutex for access to error

        int fd; ///< File descriptor for serial port

        char readBuffer[SerialPort::readBufferSize]; ///< data being read

        /// Read complete callback
        boost::function<void (const char*, size_t)> callback;
};
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
SerialPort::SerialPort(): pimpl(new SerialPortImpl)
{

}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
SerialPort::SerialPort(const std::string& devname, unsigned int baud_rate,
        boost::asio::serial_port_base::parity opt_parity,
        boost::asio::serial_port_base::character_size opt_csize,
        boost::asio::serial_port_base::flow_control opt_flow,
        boost::asio::serial_port_base::stop_bits opt_stop)
: pimpl(new SerialPortImpl)
{
    open(devname,baud_rate,opt_parity,opt_csize,opt_flow,opt_stop);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::open(const std::string& devname, unsigned int baud_rate,
        boost::asio::serial_port_base::parity opt_parity,
        boost::asio::serial_port_base::character_size opt_csize,
        boost::asio::serial_port_base::flow_control opt_flow,
        boost::asio::serial_port_base::stop_bits opt_stop)
{
    if(is_open()) close();

    set_error_status(true);//If an exception is thrown, error remains true

    struct termios new_attributes;
    speed_t speed;
    int status;

    // Open port
    pimpl->fd=::open(devname.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (pimpl->fd<0) throw(boost::system::system_error(
                boost::system::error_code(),"Failed to open port"));

    // Set Port parameters.
    status=tcgetattr(pimpl->fd,&new_attributes);
    if(status<0  || !isatty(pimpl->fd))
    {
        ::close(pimpl->fd);
        throw(boost::system::system_error(
                    boost::system::error_code(),"Device is not a tty"));
    }
    new_attributes.c_iflag = IGNBRK;
    new_attributes.c_oflag = 0;
    new_attributes.c_lflag = 0;
    new_attributes.c_cflag = (CS8 | CREAD | CLOCAL);//8 data bit,Enable receiver,Ignore modem
    /* In non canonical mode (Ctrl-C and other disabled, no echo,...) VMIN and VTIME work this way:
       if the function read() has'nt read at least VMIN chars it waits until has read at least VMIN
       chars (even if VTIME timeout expires); once it has read at least vmin chars, if subsequent
       chars do not arrive before VTIME expires, it returns error; if a char arrives, it resets the
       timeout, so the internal timer will again start from zero (for the nex char,if any)*/
    new_attributes.c_cc[VMIN]=1;// Minimum number of characters to read before returning error
    new_attributes.c_cc[VTIME]=1;// Set timeouts in tenths of second

    // Set baud rate
    switch(baud_rate)
    {
        case 50:speed= B50; break;
        case 75:speed= B75; break;
        case 110:speed= B110; break;
        case 134:speed= B134; break;
        case 150:speed= B150; break;
        case 200:speed= B200; break;
        case 300:speed= B300; break;
        case 600:speed= B600; break;
        case 1200:speed= B1200; break;
        case 1800:speed= B1800; break;
        case 2400:speed= B2400; break;
        case 4800:speed= B4800; break;
        case 9600:speed= B9600; break;
        case 19200:speed= B19200; break;
        case 38400:speed= B38400; break;
        case 57600:speed= B57600; break;
        case 115200:speed= B115200; break;
        case 230400:speed= B230400; break;
        default:
                    {
                        ::close(pimpl->fd);
                        throw(boost::system::system_error(
                                    boost::system::error_code(),"Unsupported baud rate"));
                    }
    }

    cfsetospeed(&new_attributes,speed);
    cfsetispeed(&new_attributes,speed);

    //Make changes effective
    status=tcsetattr(pimpl->fd, TCSANOW, &new_attributes);
    if(status<0)
    {
        ::close(pimpl->fd);
        throw(boost::system::system_error(
                    boost::system::error_code(),"Can't set port attributes"));
    }

    //These 3 lines clear the O_NONBLOCK flag
    status=fcntl(pimpl->fd, F_GETFL, 0);
    if(status!=-1) fcntl(pimpl->fd, F_SETFL, status & ~O_NONBLOCK);

    set_error_status(false);//If we get here, no error
    pimpl->open=true; //Port is now open

    thread t(bind(&SerialPort::do_read, this));
    pimpl->backgroundThread.swap(t);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool SerialPort::is_open() const
{
    return pimpl->open;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool SerialPort::error_status() const
{
    boost::lock_guard<boost::mutex> l(pimpl->errorMutex);
    return pimpl->error;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::close()
{
    if(!is_open()) return;

    pimpl->open=false;

    ::close(pimpl->fd); //The thread waiting on I/O should return

    pimpl->backgroundThread.join();
    if(error_status())
    {
        throw(boost::system::system_error(boost::system::error_code(),
                    "Error while closing the device"));
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::write(const char *data, size_t size)
{
    if(::write(pimpl->fd,data,size)!=size) set_error_status(true);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::write(const std::vector<char>& data)
{
    if(::write(pimpl->fd,&data[0],data.size())!=data.size())
        set_error_status(true);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::write(const std::string& s)
{
    if(::write(pimpl->fd,&s[0],s.size())!=s.size()) set_error_status(true);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
SerialPort::~SerialPort()
{
    if(is_open())
    {
        try {
            close();
        } catch(...)
        {
            //Don't throw from a destructor
        }
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::do_read()
{
    //Read loop in spawned thread
    for(;;)
    {
        int received=::read(pimpl->fd,pimpl->readBuffer,readBufferSize);
        if(received<0)
        {
            if(is_open()==false) return; //Thread interrupted because port closed
            else {
                set_error_status(true);
                continue;
            }
        }
        if(pimpl->callback) pimpl->callback(pimpl->readBuffer, received);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::readEnd(const boost::system::error_code& error,
        size_t bytes_transferred)
{
    //Not used
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::do_write()
{
    //Not used
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::write_end(const boost::system::error_code& error)
{
    //Not used
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::do_close()
{
    //Not used
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::set_error_status(bool e)
{
    boost::lock_guard<boost::mutex> l(pimpl->errorMutex);
    pimpl->error=e;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::set_read_callback(const
        function<void (const char*, size_t)>& callback)
{
    pimpl->callback=callback;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void SerialPort::clear_read_callback()
{
    pimpl->callback.clear();
}
/*  --------------------------------------------------------  */

#endif //__APPLE__

//
//Class CallbackSerialPort
//

/*  --------------------------------------------------------  */
CallbackSerialPort::CallbackSerialPort(): SerialPort()
{

}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
CallbackSerialPort::CallbackSerialPort(const std::string& devname,
        unsigned int baud_rate,
        boost::asio::serial_port_base::parity opt_parity,
        boost::asio::serial_port_base::character_size opt_csize,
        boost::asio::serial_port_base::flow_control opt_flow,
        boost::asio::serial_port_base::stop_bits opt_stop)
:SerialPort(devname,baud_rate,opt_parity,opt_csize,opt_flow,opt_stop)
{

}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void CallbackSerialPort::set_callback(const
        boost::function<void (const char*, size_t)>& callback)
{
    set_read_callback(callback);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void CallbackSerialPort::clear_callback()
{
    clear_read_callback();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
CallbackSerialPort::~CallbackSerialPort()
{
    clear_read_callback();
}
/*  --------------------------------------------------------  */
