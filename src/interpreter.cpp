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

Interpreter::Interpreter(Glib::ustring *portName)
{
    this->inOutInterface = new InOutInterface(portName);
}

/*InOutLog *Interpreter::interpret(Glib::ustring *text, std::unordered_map<std::string, std::string> *alias)
{
 //           Log::LogWarn(LEVEL_LOG_WARNING, "Unable to interpret because specific interpreter was not set", __FILE__, __LINE__);
            std::cout << "not working" << std::endl;

            return NULL;
}*/
