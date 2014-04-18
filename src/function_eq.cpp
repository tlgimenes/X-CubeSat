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

#include <typeinfo>

#include "function_eq.hpp"
#include "function_variable_int.hpp"
#include "function_variable_string.hpp"
#include "function_variable_bool.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
FunctionEq::FunctionEq(FunctionVariableInt *int1, FunctionVariableInt *int2, Terminal *term, std::unordered_map<std::string, Function*> *variables):Function(term, variables)
{
    this->params.push_back(int1);
    this->params.push_back(int2);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor: Imitates the form of this function in the
 * definition of the language
 */
FunctionEq::FunctionEq(std::vector<XCubeSatToken*> *tokens, Terminal *term, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*):Function(term, variables)
{
    if(tokens->empty()) throw new std::exception();

    XCubeSatToken *t = tokens->front();

    tokens->erase(tokens->begin());

    Function *f;

    switch(t->get_type()) {
        case STRING:
            f = new FunctionVariableString(t->get_value_str());
            break;
        case VARIABLE:
            if(variables->find(*t->get_value_str()) == variables->end()) 
                throw new std::bad_typeid();
            f = (*variables)[*t->get_value_str()];
            if(typeid(*f) != typeid(FunctionVariableInt) && typeid(*f) != typeid(FunctionVariableBool))
                throw new std::bad_typeid();
            break;
        default:
            throw new std::bad_typeid();
    }
    this->params.push_back(f);

    t = tokens->front();

    tokens->erase(tokens->begin());

    switch(t->get_type()) {
        case STRING:
            f = new FunctionVariableString(t->get_value_str());
            break;
        case VARIABLE:
            if(variables->find(*t->get_value_str()) == variables->end()) 
                throw new std::bad_typeid();
            f = (*variables)[*t->get_value_str()];
            if(typeid(*f) != typeid(FunctionVariableInt) && typeid(*f) != typeid(FunctionVariableBool))
                throw new std::bad_typeid();
            break;
        default:
            throw new std::bad_typeid();
    }
    this->params.push_back(f);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Runs the function of the language
 */
Function *FunctionEq::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    //std::cout << "EQ(";
    Function *par1 = this->params[0]->run(runQueue, satName);
    //std::cout << ",";
    Function *par2 = this->params[1]->run(runQueue, satName);
    //std::cout << ")";

    if(typeid(*par1) == typeid(FunctionVariableInt) && typeid(*par2) == typeid(FunctionVariableInt)) {
       FunctionVariableInt* int1 = static_cast<FunctionVariableInt*>(par1);
       FunctionVariableInt* int2 = static_cast<FunctionVariableInt*>(par2);

       if(int1->get_value() == int2->get_value())
           return new FunctionVariableBool(true);
       else
           return new FunctionVariableBool(false);
    }
    else if(typeid(*par1) == typeid(FunctionVariableBool) && typeid(*par2) == typeid(FunctionVariableBool)) {
        FunctionVariableBool *bool1 = static_cast<FunctionVariableBool*>(par1);
        FunctionVariableBool *bool2 = static_cast<FunctionVariableBool*>(par2);

        if(bool1->get_value() == bool2->get_value())
            return new FunctionVariableBool(true);
        else
            return new FunctionVariableBool(false);
    }
    else 
        throw new std::bad_typeid();

    return NULL;
}
/*  --------------------------------------------------------  */
