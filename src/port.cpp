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
    if(this->fd == -1) Log::LogWarn(LEVEL_LOG_ERROR, "The Serial/USB port could not be opened!", __FILE__, __LINE__);
    else {
        fcntl(this->fd, F_SETFL, FNDELAY); //NON BLOCKING READING 
    }

    return;
}

// Configure port with default configurations
void Port::Configure() {
    struct termios options;

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
        Log::LogWarn(LEVEL_LOG_WARNING, "Failled to change the configurations in the port", __FILE__, __LINE__);
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

void Port::Read(char *buff, size_t count)
{
    int nRead;

    nRead = read(this->fd, (void*)buff, count);

    if(nRead < 0)
        Log::LogWarn(LEVEL_LOG_WARNING, "The port couldn't be read, maybe some data was lost", NULL, -1);

    return;
}
