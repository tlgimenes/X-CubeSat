/*
 * MAIN: 
 *
 */

#include "log.hpp"
#include "port.hpp"

#include <string>

int main(int argc, char * argv[])
{
    const char * port = "/dev/ttyS0";

    if(argc > 0)
        port = argv[argc-1];

    Port serial(port);

    serial.Write("Hello World!\n", 13);

    char buff[24];

    serial.Read(buff, 24);

    return 0;
}
