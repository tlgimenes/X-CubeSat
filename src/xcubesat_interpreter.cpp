/*
 * CLASS XCUBESAT_INTERPRETER : This class defines 
 * methods for interpreting a command file.
 *
 */

#include <iostream>
#include "xcubesat_interpreter.hpp"

XCubeSatInterpreter::XCubeSatInterpreter(InOutInterface *inOutInterface) : Interpreter(inOutInterface){}

XCubeSatInterpreter::XCubeSatInterpreter(std::string *portName) : Interpreter(portName){}

InOutLog *XCubeSatInterpreter::Interpret(std::string *text, std::unordered_map<std::string, std::string> *alias)
{
    std::cout << "interpreting" << std::endl;

    return new InOutLog(SEND, NULL, true);
}
