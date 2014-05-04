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

#include <iostream>
#include <typeinfo>

#include "xcubesat_compiler.hpp"
#include "function_send.hpp"
#include "function_save.hpp"
#include "function_if.hpp"
#include "function_declare.hpp"
#include "function_eq.hpp"
#include "function_ll.hpp"
#include "function_leq.hpp"
#include "function_file.hpp"
#include "function_appenddate.hpp"
#include "function_format.hpp"
#include "function_receive.hpp"
#include "function_variable_string.hpp"
#include "function_variable_bool.hpp"
#include "function_set.hpp"
#include "function_declare.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
FunctionIf::FunctionIf(FunctionVariableBool *boolean, Terminal *term, std::unordered_map<std::string, Function*> *variables):Function(term, variables)
{
    this->params.push_back(boolean);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor: Imitates the form of this function in the
 * definition of the language
 */
FunctionIf::FunctionIf(std::vector<XCubeSatToken*> *tokens, Terminal *term, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*):Function(term, variables)
{
    if(tokens->empty() || tokens->size() < 2) throw new std::bad_typeid();

    XCubeSatToken *t = tokens->front();

    tokens->erase(tokens->begin());

    Function *f;

    switch(t->get_type()) {
        case SEND:
            f = new FunctionSend(tokens, this->term, variables);
            break;
        case SAVE:
            f = new FunctionSave(tokens, this->term, variables);
            break;
        case IF:
            f = new FunctionIf(tokens, this->term, variables);
            break;
        case EQ:
        case EQUALS:
            f = new FunctionEq(tokens, this->term, variables);
            break;
        case LL:
            f = new FunctionLl(tokens, this->term, variables);
            break;
        case LEQ:
            f = new FunctionLeq(tokens, this->term, variables);
            break;
        case VARIABLE:
            if(variables->find(*t->get_value_str()) == variables->end()) 
                throw new std::bad_typeid();
            f = (*variables)[*t->get_value_str()];
            if(typeid(*f) != typeid(FunctionVariableBool))
                throw new std::bad_typeid();
            break;
        default:
            throw new std::bad_typeid();
    }
    this->params.push_back(f);

    if(tokens->empty()) throw new std::bad_typeid();
    t = tokens->front();
    tokens->erase(tokens->begin());

    if(t->get_type() != LPAREN) throw new std::bad_typeid();

    while(!tokens->empty() && tokens->front()->get_type() != RPAREN) {
        t = tokens->front();

        tokens->erase(tokens->begin());

        switch(t->get_type()) {
            case SEND:
                f = new FunctionSend(tokens, this->term, variables);
                break;
            case SAVE:
                f = new FunctionSave(tokens, this->term, variables);
                break;
            case IF:
                f = new FunctionIf(tokens, this->term, variables);
                break;
            case EQ:
                f = new FunctionEq(tokens, this->term, variables);
                break;
            case LL:
                f = new FunctionLl(tokens, this->term, variables);
                break;
            case LEQ:
                f = new FunctionLeq(tokens, this->term, variables);
                break;
            case FILE2:
                f = new FunctionFile(tokens, this->term, variables);
                break;
            case APPENDDATE:
                f = new FunctionAppendDate(tokens, this->term, variables);
                break;
            case FORMAT:
                f = new FunctionFormat(tokens, this->term, variables);
                break;
            case RECEIVE:
                f = new FunctionReceive(tokens, this->term, variables);
                break;
            case SET:
                f = new FunctionSet(tokens, this->term, variables);
                break;
            case DECLARE:
                f = new FunctionDeclare(tokens, this->term, variables);
                break;
            case EQUALS:
                /* TODO */
                break;
            case STRING:
            default:
                throw new std::bad_typeid();
        }
        this->params.push_back(f);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Runs the function of the language
 */
Function *FunctionIf::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    bool res;

//std::cout << "IF(";
    Function *f = this->params[0]->run(runQueue, satName);
//std::cout << ")";

    if(f != NULL && typeid(*f) == typeid(FunctionVariableBool)) {
        FunctionVariableBool *f2 = static_cast<FunctionVariableBool*>(f);
        res = f2->get_value();
        if(res && this->params.size() > 1) {
//std::cout << std::endl << "{" << std::endl;
            for(unsigned int i=1; i < this->params.size(); i++) {
                this->params[i]->run(runQueue, satName);
//std::cout << std::endl;
            }
//std::cout << "}" << std::endl;
        }
    }

    return new FunctionVariableBool(true);
}
/*  --------------------------------------------------------  */
