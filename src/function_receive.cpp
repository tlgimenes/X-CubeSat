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
#include <ctime>

#include "function_receive.hpp"
#include "function_file.hpp"
#include "function_appenddate.hpp"
#include "function_format.hpp"
#include "function_variable_string.hpp"
#include "function_variable_data.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
FunctionReceive::FunctionReceive(FunctionVariableString *str, Terminal *term, std::unordered_map<std::string, Function*> *variables):Function(term, variables)
{
    this->params.push_back(str);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor: Imitates the form of this function in the
 * definition of the language
 */
FunctionReceive::FunctionReceive(std::vector<XCubeSatToken*> *tokens, Terminal *term, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*):Function(term, variables)
{
    if(tokens->empty()) throw new std::exception();

    XCubeSatToken *t = tokens->front();

    tokens->erase(tokens->begin());

    Function *f;

    switch(t->get_type()) {
        case FILE2:
            f = new FunctionFile(tokens, this->term, variables);
            break;
        case APPENDDATE:
            f = new FunctionAppendDate(tokens, this->term, variables);
            break;
        case FORMAT:
            f = new FunctionFormat(tokens, this->term, variables);
            break;
        case STRING:
            f = new FunctionVariableString(t->get_value_str());
            break;
        case VARIABLE:
            if(variables->find(*t->get_value_str()) == variables->end()) 
                throw new std::bad_typeid();
            f = (*variables)[*t->get_value_str()];
            if(typeid(*f) != typeid(FunctionVariableString))
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
Function *FunctionReceive::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    FunctionVariableData *data = NULL;
    std::string str;
    time_t raw_time;

    time(&raw_time);
    struct tm *ptm = gmtime(&raw_time);

   // std::cout << "Receive(";
    Function *format = this->params[0]->run(runQueue, satName);
    //std::cout << ")";

    if(format != NULL && typeid(*format) == typeid(FunctionVariableString)) {
        FunctionVariableString *format_str = static_cast<FunctionVariableString*>(format);

        if(this->term->read_from_device(&str)) {
            data = new FunctionVariableData(str, *format_str->get_value(), (ptm->tm_hour)*10000 + ptm->tm_min*100 + ptm->tm_sec);
        }
    }

    return data;
}
/*  --------------------------------------------------------  */
