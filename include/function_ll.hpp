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

#ifndef FUNCTION_LL_HPP
#define FUNCTION_LL_HPP

#include "function.hpp"
#include "function_variable_int.hpp"
#include "xcubesat_token.hpp"
#include "terminal.hpp"

class FunctionLl : public Function
{
    public:
        FunctionLl(FunctionVariableInt *int1, FunctionVariableInt *int2, Terminal *term, std::unordered_map<std::string, Function*> *variables);
        FunctionLl(std::vector<XCubeSatToken*> *tokens, Terminal *term, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*);

        Function *run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*);
};

#endif
