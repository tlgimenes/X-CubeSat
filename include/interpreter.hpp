/*
 * CLASS INTERPRETER : This class defines methods for
 * interpreting a command file.
 *
 */

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <gtkmm.h>
#include <unordered_map>
#include "in_out_interface.hpp"
#include "in_out_log.hpp"

class Interpreter
{
    protected:
        InOutInterface *inOutInterface;

    public:
        Interpreter(InOutInterface *);
        Interpreter(Glib::ustring *portName);

        virtual InOutLog *Interpret(Glib::ustring *text, std::unordered_map<std::string, std::string> *alias){return NULL;}

};

#endif
