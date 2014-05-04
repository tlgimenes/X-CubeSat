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

#include <typeinfo>

#include "function_declare.hpp"
#include "function_file.hpp"
#include "function_appenddate.hpp"
#include "function_format.hpp"
#include "function_variable.hpp"
#include "function_variable_bool.hpp"
#include "function_variable_data.hpp"
#include "function_variable_int.hpp"
#include "function_variable_string.hpp"

FunctionDeclare::FunctionDeclare(FunctionVariableString *type, FunctionVariableString *name, Terminal *term, std::unordered_map<std::string, Function*> *var):Function(term, variables)
{
    this->params.push_back(type);
    this->params.push_back(name);
}

FunctionDeclare::FunctionDeclare(std::vector<XCubeSatToken*> *tokens, Terminal *term, std::unordered_map<std::string, Function*> *var) throw(std::bad_typeid*):Function(term, variables)
{
    if(tokens->empty() || tokens->size() < 2) throw new std::bad_typeid();

    XCubeSatToken *t = tokens->front();

    tokens->erase(tokens->begin());

    Function *f;
    Glib::ustring *type;

    switch(t->get_type()) {
        case STRING:
        case VARIABLE:
            type = t->get_value_str();
            type->uppercase();
            if(!type->compare("BOOL")) {
                f = new FunctionVariableBool(false);}
            else if(!type->compare("DATA")) {
                f = new FunctionVariableData(NULL, NULL, 0);}
            else if(!type->compare("INT")) {
                f = new FunctionVariableInt(0);}
            else if(!type->compare("STRING")) {
                f = new FunctionVariableString(NULL); }
            else {
                throw new std::bad_typeid();}
            break;
        default:
            throw new std::bad_typeid();
    }
    this->params.push_back(f);

    if(tokens->empty()) throw new std::bad_typeid();
    t = tokens->front();
    tokens->erase(tokens->begin());

    std::pair<std::string, Function*> *p;
    switch(t->get_type()) {
        case VARIABLE:
            p = new std::pair<std::string, Function*>(*t->get_value_str(), f);
            var->insert(*p);
            break;
        default:
            throw new std::bad_typeid();
    }
}

Function *FunctionDeclare::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    //std::cout << "Declare(NULL)";

    return NULL;
}
