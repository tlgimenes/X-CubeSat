/*
 * CLASS PORT : This class connects to serial and usb ports
 * and provides some basic read write commands
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

#ifndef PORT_HPP
#define PORT_HPP

#include <fstream>

#define NO_PORT_DEFINED NULL

typedef enum portType_t
{
    SERIAL,
    USB
} portType;

class Port
{
    private:
        int fd;
        const char *portName;  //Name of the port to connec to
        const char *defaultPort;
        bool isOppenned;
        bool isConfigured;

    protected:
        void open_port();
        void configure();

    public:
        Port(const char *);
        void write_to_port(char *, unsigned int);
        void read_port(char* buff, size_t count);
        char *read_port(char delim);
        bool is_oppenned();
        bool is_configured();
        bool open_port(const char *portName, int speed);
        bool set_speed(int speed);
};

#endif
