/*
 * CLASS PORT : This class connects to serial and usb ports
 * and provides some basic read write commands
 *
 */

#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <string.h>

#include "port.hpp"
#include "log.hpp"

Port::Port(const char * portName)
{
    this->fd = -1;
    this->portName = portName;
#ifdef WINDOWS
    this->defaultPort = "COM";
#else
    this->defaultPort = "/dev/ttyS0";
#endif
    this->isOppenned = false;
    this->isConfigured = false;

    // Opens the specified port by portName
    // or a defaultPort otherwise
    this->Open();

    // Configure the port
    this->Configure();
}

void Port::Open()
{
    // Opens the port; default if not specified
    if(this->portName == NO_PORT_DEFINED) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Specified port not found, opening default port", NULL, -1);
        this->fd = open(this->defaultPort, O_RDWR | O_NOCTTY | O_NDELAY);
    } 
    else {
        this->fd = open(this->portName, O_RDWR | O_NOCTTY | O_NDELAY);
    }

    // If could not open the port
    if(this->fd == -1) {
        this->isOppenned = false;
        Log::LogWarn(LEVEL_LOG_WARNING, "The Serial/USB port could not be opened!", __FILE__, __LINE__);
    }
    else {
        this->isOppenned = true;
        fcntl(this->fd, F_SETFL, FNDELAY); //NON BLOCKING READING 
    }

    return;
}

// Configure port with default configurations
void Port::Configure() {
    struct termios options;

    if(this->isOppenned){
        // Get current configurations
        if(tcgetattr(fd, &options))
            Log::LogWarn(LEVEL_LOG_WARNING, "Failled to get the current configurations of the port", __FILE__, __LINE__);

        // Choses speed at baud 19200
        cfsetispeed(&options, B19200);  
        cfsetospeed(&options, B19200);

        options.c_cflag |= (CLOCAL | CREAD); // Enable the receiver and set local mode

        options.c_cflag &= ~CSIZE; // Mask the character size bits 
        options.c_cflag |= CS8;    // Select 8 data bits

        options.c_iflag |= (IXON | IXOFF | IXANY); // Enable Software Flow Control

        options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Enable raw input
        options.c_oflag &= ~OPOST; // Enable raw output

        // Push the new options for the port
        if(tcsetattr(fd, TCSANOW, &options)) 
            Log::LogWarn(LEVEL_LOG_WARNING, "Failled to change the configurations of the port", __FILE__, __LINE__);
        else 
            this->isConfigured = true;
    }
    else
        Log::LogWarn(LEVEL_LOG_INFO, "Tried to configure port not oppenned", __FILE__, __LINE__);
}

void Port::Write(char *data, unsigned int bytesSize)
{
    unsigned int written, dataLen;

    // Checks if valid data
    if(data != NULL) {
        dataLen = strlen(data);
    }
    else {
        Log::LogWarn(LEVEL_LOG_WARNING, "You Tried sending invalid data! The attempt was blocked!", NULL, -1);
    }

    if(this->isOppenned) {
        // Checks if data length corresponds to the specified one
        if(bytesSize == dataLen) {
            written = write(this->fd, data, bytesSize); //WRITE !
            if(written != bytesSize) // Checks if written data equals the true size of the data
                Log::LogWarn(LEVEL_LOG_WARNING, "An error has occured and the data sent does not correspond to the data desired", NULL, -1);
        }
        else {
            Log::LogWarn(LEVEL_LOG_WARNING, "The size of the data that you are sending does not correspond to the data size!", NULL, -1);
        }
    }
    else {
        Log::LogWarn(LEVEL_LOG_WARNING, "Tried to write to not oppenned port", __FILE__, __LINE__);
    }
}

void Port::Read(char *buff, size_t count)
{
    int nRead;

    if(this->isOppenned) {
    nRead = read(this->fd, (void*)buff, count);

    if(nRead < 0)
        Log::LogWarn(LEVEL_LOG_WARNING, "The port couldn't be read, maybe some data was lost", NULL, -1);
    }
    else {
        Log::LogWarn(LEVEL_LOG_WARNING, "Tried to read to not oppenned port", __FILE__, __LINE__);
    }

    return;
}

bool Port::IsOppenned()
{
    return this->isOppenned;
}

bool Port::IsConfigured()
{
    return this->isConfigured;
}

bool Port::Open(const char*portName, int speed)
{
    this->fd = -1;
    this->portName = portName;
    this->isConfigured = false;
    this->isOppenned = false;

    // Opens the specified port by portName
    // or a defaultPort otherwise
    this->Open();

    // Configure the port
    this->SetSpeed(speed);

    return (this->isOppenned);
}

#define CASE(int_speed, speed_t) \
    case int_speed: \
        newSpeed = speed_t; \
        break;

// Configure port with the speed speed
bool Port::SetSpeed(int speed) {
    struct termios options;
    speed_t newSpeed;
    int success = 0;

    this->isConfigured = false;
    if(this->isOppenned){
        // Get current configurations
        if(tcgetattr(this->fd, &options)) {
            Log::LogWarn(LEVEL_LOG_WARNING, "Failled to get the current configurations of the port", __FILE__, __LINE__);
            return false;
        }
        else {
            switch(speed){
                CASE(0,     B0);
                CASE(50,    B50);
                CASE(75,    B75);
                CASE(110,   B110);
                CASE(134,   B134)
                CASE(150,   B150);
                CASE(200,   B200);
                CASE(300,   B300);
                CASE(600,   B600);
                CASE(1200,  B1200);
                CASE(1800,  B1800);
                CASE(2400,  B2400);
                CASE(4800,  B4800);
                CASE(9600,  B9600);
                CASE(19200, B19200);
                CASE(38400, B38400);
                CASE(57600, B57600);
                CASE(115200,B115200);
                CASE(230400,B230400);
                default:
                    Log::LogWarn(LEVEL_LOG_WARNING, "Unsupported port speed", __FILE__, __LINE__);
                    return false;
            }
            // Choses speed at baud 19200
            success = cfsetispeed(&options, newSpeed);  
            success += cfsetospeed(&options, newSpeed);
            if(success != 0) {
                Log::LogWarn(LEVEL_LOG_WARNING, "Could not set the port speed", __FILE__, __LINE__);
                return false;
            }
            else
                this->isConfigured = true;
        }
    }
    else {
        Log::LogWarn(LEVEL_LOG_INFO, "Tried to configure port not oppenned", __FILE__, __LINE__);
        return false;
    }
    return true;
}

