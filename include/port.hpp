/*
 * CLASS PORT : This class connects to serial and usb ports
 * and provides some basic read write commands
 *
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
        bool is_oppenned();
        bool is_configured();
        bool open_port(const char *portName, int speed);
        bool set_speed(int speed);
};

#endif
