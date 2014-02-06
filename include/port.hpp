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
        void Open();
        void Configure();

    public:
        Port(const char *);
        void Write(char *, unsigned int);
        void Read(char* buff, size_t count);
        bool IsOppenned();
        bool IsConfigured();
        bool Open(const char *portName, int speed);
        bool SetSpeed(int speed);
};

#endif
