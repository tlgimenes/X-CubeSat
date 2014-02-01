/*
 * CLASS IN_OUT_INTERFACE : This class provides 
 * methods for operating input output for the program.
 *
 */

#ifndef IN_OUT_INTERFACE_HPP
#define IN_OUT_INTERFACE_HPP

#include "port.hpp"
#include "in_out_log.hpp"

class InOutInterface
{
    private:
        Port * port;

    public:
        InOutInterface();
        InOutInterface(std::string *portName);
        InOutLog * Write(std::string *data);
        InOutLog * Read(size_t count);
};

#endif
