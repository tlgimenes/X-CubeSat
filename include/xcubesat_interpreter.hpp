/*
 * CLASS XCUBESAT_INTERPRETER : This class defines 
 * methods for interpreting a command file.
 *
 */

#ifndef XCUBESAT_INTERPRETER_HPP
#define XCUBESAT_INTERPRETER_HPP

#include <unordered_map>
#include "in_out_log.hpp"
#include "in_out_interface.hpp"
#include "interpreter.hpp"

class XCubeSatInterpreter : virtual public Interpreter
{
    public:
        XCubeSatInterpreter(InOutInterface *);
        XCubeSatInterpreter(std::string *portName);

        InOutLog *Interpret(std::string *text, std::unordered_map<std::string, std::string> *alias);
};

#endif
