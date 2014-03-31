/*
 * CLASS INTERPRETER : This class defines methods for
 * interpreting a command file.
 *
 */

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <gtkmm.h>
#include <unordered_map>
#include <iostream>

#include "in_out_interface.hpp"
#include "in_out_log.hpp"
#include "log.hpp"

class Interpreter
{
    protected:
        InOutInterface *inOutInterface;

    public:
        Interpreter(InOutInterface *);
        Interpreter(Glib::ustring *portName);

        virtual InOutLog *interpret(Glib::ustring *text, std::unordered_map<std::string, std::string> *alias, Glib::ustring *satName)
        {
            Log::LogWarn(LEVEL_LOG_WARNING, "Unable to interpret because specific interpreter was not set", __FILE__, __LINE__);

            return NULL;
        }

        virtual bool *are_there_syntax_errors(Glib::ustring *text, std::unordered_map<std::string, std::string> *alias) 
        {
            Log::LogWarn(LEVEL_LOG_WARNING, "Unable to interpret because specific interpreter was not set", __FILE__, __LINE__);

            return NULL;
        }
};

#endif
