/*
 * CLASS XCUBESAT_INTERPRETER : This class defines 
 * methods for interpreting a command file.
 *
 */
/* X-CubeSat Controler: Real-time communication with satellite program

 Copyright (C)  2014 - Tiago Lobato Gimenes

 Authors: Tiago Lobato Gimenes <tlgimenes@gmail.com>

 Comments, questions and bugreports should be submitted via
 https://github.com/tlgimenes/X-CubeSat
 More details can be found at the project home page:

 https://github.com/tlgimenes/X-CubeSat

 This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

 This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
along with this program; if not, visit http://www.fsf.org/
*/

#ifndef XCUBESAT_INTERPRETER_HPP
#define XCUBESAT_INTERPRETER_HPP

#include <unordered_map>

#include "in_out_log.hpp"
#include "terminal.hpp"
#include "interpreter.hpp"
#include "xcubesat_compiler.hpp"

class XCubeSatInterpreter : virtual public Interpreter
{
    private:
        XCubeSatCompiler *compiler;
        std::vector<Function*> runQueue;

    public:
        XCubeSatInterpreter(Terminal *);

        InOutLog *interpret(Glib::ustring *text, std::unordered_map<std::string, std::string> *alias, Glib::ustring *satName);

        bool *are_there_syntax_errors(Glib::ustring *text, std::unordered_map<std::string, std::string> *alias);
};

#endif
