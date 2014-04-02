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

#include "xcubesat_token.hpp"
#include "function_appenddate.hpp"
#include "function_eq.hpp"
#include "function_file.hpp"
#include "function_format.hpp"
#include "function_if.hpp"
#include "function_leq.hpp"
#include "function_ll.hpp"
#include "function_receive.hpp"
#include "function_save.hpp"
#include "function_send.hpp"
#include "function_set.hpp"
#include "function_variable_bool.hpp"
#include "function_variable_int.hpp"
#include "function_variable_data.hpp"
#include "function_variable_string.hpp"

/*  --------------------------------------------------------  */
/* Constructor: Imitates the form of this function in the
 * definition of the language
 */
FunctionSet::FunctionSet(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*):Function(interface, variables)
{
    if(tokens->empty()) throw new std::exception();

    XCubeSatToken *t = tokens->front();
    tokens->erase(tokens->begin());

    Function *f, *v;

    switch(t->get_type()) {
        case VARIABLE:
            if(variables->find(*t->get_value_str()) == variables->end()) 
                throw new std::bad_typeid();
            f = (*variables)[*t->get_value_str()];
            break;
        default:
            throw new std::bad_typeid();
    }
    this->params.push_back(f);

    t = tokens->front();
    tokens->erase(tokens->begin());

    switch(t->get_type()) {
        case SEND:
            if(typeid(*f) != typeid(FunctionVariableBool))
                throw new std::bad_typeid();
            f = new FunctionSend(tokens, interface, variables);
            break;
        case SAVE:
            if(typeid(*f) != typeid(FunctionVariableBool))
                throw new std::bad_typeid();
            f = new FunctionSave(tokens, interface, variables);
            break;
        case IF:
            if(typeid(*f) != typeid(FunctionVariableBool))
                throw new std::bad_typeid();
            f = new FunctionIf(tokens, interface, variables);
            break;
        case EQ:
            if(typeid(*f) != typeid(FunctionVariableBool))
                throw new std::bad_typeid();
            f = new FunctionEq(tokens, interface, variables);
            break;
        case LL:
            if(typeid(*f) != typeid(FunctionVariableBool))
                throw new std::bad_typeid();
            f = new FunctionLl(tokens, interface, variables);
            break;
        case LEQ:
            if(typeid(*f) != typeid(FunctionVariableBool))
                throw new std::bad_typeid();
            f = new FunctionLeq(tokens, interface, variables);
            break;
        case FILE2:
            if(typeid(*f) != typeid(FunctionVariableString))
                throw new std::bad_typeid();
            f = new FunctionFile(tokens, interface, variables);
            break;
        case APPENDDATE:
            if(typeid(*f) != typeid(FunctionVariableString))
                throw new std::bad_typeid();
            f = new FunctionAppendDate(tokens, interface, variables);
            break;
        case FORMAT:
            if(typeid(*f) != typeid(FunctionVariableString))
                throw new std::bad_typeid();
            f = new FunctionFormat(tokens, interface, variables);
            break;
        case STRING:
            if(typeid(*f) != typeid(FunctionVariableString))
                throw new std::bad_typeid();
            f = new FunctionVariableString(t->get_value_str());
            break;
        case RECEIVE:
            if(typeid(*f) != typeid(FunctionVariableData))
                throw new std::bad_typeid();
            f = new FunctionReceive(tokens, interface, variables);
            break;
        case VARIABLE:
            if(variables->find(*t->get_value_str()) == variables->end()) {
                if(typeid(*f) == typeid(FunctionVariableInt)){
                    f = new FunctionVariableInt(std::stoi(*t->get_value_str()));
                }
                else if(typeid(*f) == typeid(FunctionVariableBool)){
                    if(!t->get_value_str()->compare("TRUE"))
                        f = new FunctionVariableBool(true);
                    else if(!t->get_value_str()->compare("FALSE"))
                        f = new FunctionVariableBool(false);
                    else
                        throw new std::bad_typeid();
                }
                else if(typeid(*f) == typeid(FunctionVariableString)){
                    f = new FunctionVariableString(t->get_value_str());
                }
                else
                    throw new std::bad_typeid();
            }
            else {
                v = (*variables)[*t->get_value_str()];
                if(v == NULL || f == NULL || typeid(*v) != typeid(*f)) throw new std::bad_typeid();
                f = v;
            }
            break;
        case LPAREN:
        case RPAREN:
        case DECLARE:
        case SET:
        default:
            throw new std::bad_typeid();
    }
    this->params.push_back(f);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Runs the function of the language
 */
Function *FunctionSet::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    Function *var = this->params[0];
    Function *as  = this->params[1];
std::cout << "SET(";
    if(var != NULL && as != NULL) {
        as  = as->run(runQueue, satName);
        if(as != NULL && var != NULL && typeid(*var) == typeid(*as)) {
            if(typeid(*var) == typeid(FunctionVariableBool)) {
                FunctionVariableBool *var2 = static_cast<FunctionVariableBool*>(var);
                var2->set_value(static_cast<FunctionVariableBool*>(as->run(runQueue, satName)));
std::cout << ")";
            }
            else if(typeid(*var) == typeid(FunctionVariableData)) {
                FunctionVariableData *var2 = static_cast<FunctionVariableData*>(var);
                var2->set_value(static_cast<FunctionVariableData*>(as->run(runQueue, satName)));
std::cout << ")";
            }
            else if(typeid(*var) == typeid(FunctionVariableInt)) {
                FunctionVariableInt *var2 = static_cast<FunctionVariableInt*>(var);
                var2->set_value(static_cast<FunctionVariableInt*>(as->run(runQueue, satName)));
std::cout << ")";
            } 
            else if(typeid(*var) == typeid(FunctionVariableString)) {
                FunctionVariableString *var2 = static_cast<FunctionVariableString*>(var);
                var2->set_value(static_cast<FunctionVariableString*>(as->run(runQueue, satName)));
std::cout << ")";
            }
            else 
                throw new std::bad_typeid();
        }
        else throw new std::bad_typeid();
    }
    else throw new std::bad_typeid();
    return NULL;
}

