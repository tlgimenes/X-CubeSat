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

#include "function_save.hpp"
#include "function_receive.hpp"
#include "function_file.hpp"
#include "function_appenddate.hpp"
#include "function_format.hpp"
#include "function_variable_string.hpp"
#include "function_variable_bool.hpp"
#include "function_variable_data.hpp"
#include "data_base.hpp"
#include "log.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
FunctionSave::FunctionSave(FunctionVariableData *dat, FunctionVariableString *str, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables):Function(interface, variables)
{
    this->params.push_back(dat);
    this->params.push_back(str);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor: Imitates the form of this function in the
 * definition of the language
 */
FunctionSave::FunctionSave(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*):Function(interface, variables)
{
    if(tokens->empty()) throw new std::exception();

    XCubeSatToken *t = tokens->front();

    tokens->erase(tokens->begin());

    Function *f;

    switch(t->get_type()) {
        case RECEIVE:
            f = new FunctionReceive(tokens, this->interface, variables);
            break;
        case VARIABLE:
            if(variables->find(*t->get_value_str()) == variables->end()) 
                throw new std::bad_typeid();
            f = (*variables)[*t->get_value_str()];
            if(typeid(*f) != typeid(FunctionVariableData))
                throw new std::bad_typeid();
            break;
        default:
            throw new std::bad_typeid();
    }
    this->params.push_back(f);

    t = tokens->front();

    tokens->erase(tokens->begin());

    switch(t->get_type()) {
        case FILE2:
            f = new FunctionFile(tokens, this->interface, variables);
            break;
        case APPENDDATE:
            f = new FunctionAppendDate(tokens, this->interface, variables);
            break;
        case FORMAT:
            f = new FunctionFormat(tokens, this->interface, variables);
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
Function *FunctionSave::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    FunctionVariableBool *res = NULL;

   // std::cout << "Save(";
    Function *par1 = this->params[0]->run(runQueue, satName);
   // std::cout << ",";
    Function *par2 = this->params[1]->run(runQueue, satName);
   // std::cout << ")";

    if(par1 != NULL && par2 != NULL && typeid(par1) == typeid(FunctionVariableData) && typeid(par2) == typeid(FunctionVariableString)) {
        FunctionVariableData   *data = static_cast<FunctionVariableData*>(par1);
        FunctionVariableString *file = static_cast<FunctionVariableString*>(par2);

        if(!data->get_value_format()->compare("WOD")) {
            wod_data d;
            d.dataNickname = file->get_value();
            d.rawContent = data->get_value_received();
            d.timeStamp = data->get_value_date();
            d.satName = satName;
            res = new FunctionVariableBool(DataBase::save_wod_data(d));
        }
        else if(!data->get_value_format()->compare("SCI")) {
            sci_data d;
            d.dataNickname = file->get_value();
            d.rawContent = data->get_value_received();
            d.timeStamp = data->get_value_date();
            d.satName = satName;
            res = new FunctionVariableBool(DataBase::save_sci_data(d));
        }
        else {
            res = new FunctionVariableBool(false);
            Log::LogWarn(LEVEL_LOG_INFO, "Data not salved because of error in file format", __FILE__, __LINE__);
        }
    }

    return res;
}
/*  --------------------------------------------------------  */
