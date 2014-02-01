/*
 * CLASS INTERPRETER : This class defines methods for
 * interpreting a command file.
 *
 */

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <unordered_map>
#include "in_out_interface.hpp"
#include "in_out_log.hpp"

class Interpreter
{
    protected:
        InOutInterface *inOutInterface;

    public:
        Interpreter(InOutInterface *);
        Interpreter(std::string *portName);

        virtual InOutLog *Interpret(std::string *text, std::unordered_map<std::string, std::string> *alias){return NULL;}

};

#endif
