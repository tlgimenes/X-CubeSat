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

#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <vector>
#include <iostream>
#include <string>
#include <typeinfo>
#include <unordered_map>

#include "terminal.hpp"

class Function 
{
    protected:
        std::vector<Function*> params;
        std::unordered_map<std::string, Function*> *variables;
        Terminal *term;

    public:
        Function();
        Function(Terminal *term, std::unordered_map<std::string, Function*> *variables);
        virtual Function *run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*) {return NULL;}
};

#endif
