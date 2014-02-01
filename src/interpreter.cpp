/*
 * CLASS INTERPRETER : This class defines methods for
 * interpreting a command file.
 *
 */

#include "interpreter.hpp"

Interpreter::Interpreter(InOutInterface *inOutInterface)
{
    this->inOutInterface = inOutInterface;
}

Interpreter::Interpreter(std::string *portName)
{
    this->inOutInterface = new InOutInterface(portName);
}


