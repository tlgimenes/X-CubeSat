/* X-CubeSat Controller: Real-time communication with satellite program

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

#ifndef XCUBESAT_COMPILER_HPP
#define XCUBESAT_COMPILER_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <gtkmm.h>
#include <typeinfo>

#include "function.hpp"
#include "xcubesat_token.hpp"
#include "terminal.hpp"

class XCubeSatCompiler 
{
    private:
        std::vector<XCubeSatToken*> *tokens;
        Glib::ustring log;
        bool error;

    protected:
        std::vector<XCubeSatToken*> *tokenizer(std::stringstream *fileString);
        std::stringstream *replace_alias(std::unordered_map<std::string, std::string> *alias, std::stringstream *fileString);

    public:
        XCubeSatCompiler();
        
        std::vector<Function*> *compile(std::unordered_map<std::string, std::string> *alias, std::stringstream *fileString, Terminal *term) throw(std::bad_typeid*);

        bool are_there_syntax_errors();
        Glib::ustring get_syntax_errors();
};

#endif
