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
#include "xcubesat_compiler.hpp"

class XCubeSatInterpreter : virtual public Interpreter
{
    private:
        XCubeSatCompiler *compiler;
        std::vector<Function*> runQueue;

    public:
        XCubeSatInterpreter(InOutInterface *);
        XCubeSatInterpreter(Glib::ustring *portName);

        InOutLog *interpret(Glib::ustring *text, std::unordered_map<std::string, std::string> *alias, Glib::ustring *satName);

        bool *are_there_syntax_errors(Glib::ustring *text, std::unordered_map<std::string, std::string> *alias);
};

#endif
