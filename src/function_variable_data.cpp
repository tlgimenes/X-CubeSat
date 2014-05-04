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

#include "function_variable_data.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
FunctionVariableData::FunctionVariableData():Function()
{
    /*
     * Nothing to do here;
     */
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor
 */
FunctionVariableData::FunctionVariableData(FunctionVariableData* d): Function()
{
    this->received_data = *d->get_value_received();
    this->format        = *d->get_value_format();
    this->date          =  d->get_value_date();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor
 */
FunctionVariableData::FunctionVariableData(Glib::ustring str, Glib::ustring format, unsigned int date):Function()
{
    this->received_data = str;
    this->format = format;
    this->date   = date;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Sets the value of this variable
 */
void FunctionVariableData::set_value(FunctionVariableData* d)
{
    this->received_data = *d->get_value_received();
    this->format        = *d->get_value_format();
    this->date          =  d->get_value_date();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Sets the value of this variable
 */
void FunctionVariableData::set_value(Glib::ustring str, Glib::ustring format, unsigned int date)
{
    this->received_data = str;
    this->format = format;
    this->date   = date;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the value of the received value
 */
Glib::ustring *FunctionVariableData::get_value_received()
{
    return &(this->received_data);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the value of the format
 */
Glib::ustring *FunctionVariableData::get_value_format()
{
    return &(this->format);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the date of that this variable was created
 */
unsigned int FunctionVariableData::get_value_date()
{
    return this->date;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Runs the function of the language
 */
Function *FunctionVariableData::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    //std::cout << "VariableData";

    return this;
}
/*  --------------------------------------------------------  */
