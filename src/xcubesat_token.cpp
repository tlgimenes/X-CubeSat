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

#include "xcubesat_token.hpp"

/*  --------------------------------------------------------  */
/*  Constructors 
 */
XCubeSatToken::XCubeSatToken(token t)
{
    this->type = t;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Set the token a type and the data in case of string and 
 * variable
 */
XCubeSatToken::XCubeSatToken(Glib::ustring *str)
{
    if(!str->compare("SEND")) 
        type = SEND; 
    else if(!str->compare("RECEIVE")) 
        type = RECEIVE; 
    else if(!str->compare("SAVE")) 
        type = SAVE; 
    else if(!str->compare("IF")) 
        type= IF; 
    else if(!str->compare("DECLARE")) 
        type= DECLARE; 
    else if(!str->compare("EQ")) 
        type= EQ; 
    else if(!str->compare("LL")) 
        type= LL; 
    else if(!str->compare("LEQ")) 
        type= LEQ; 
    else if(!str->compare("FILE")) 
        type= FILE2; 
    else if(!str->compare("APPENDDATE")) 
        type= APPENDDATE; 
    else if(!str->compare("FORMAT")) 
        type= FORMAT; 
    else if(!str->compare("RECEIVE"))
        type = RECEIVE;
    else if(!str->compare("SET"))
        type = SET;
    else if(!str->compare("=")) 
        type = EQUALS;
    else if(!str->compare("{"))
        type = LPAREN;
    else if(!str->compare("}"))
        type = RPAREN;
    else if((*str)[0] == '"'){
        str->erase(str->begin());
        type = STRING;
        this->data.sdata = new Glib::ustring(*str);
    }
    else {
        type = VARIABLE;
        this->data.sdata = new Glib::ustring(*str);
    }
//    std::cout << "the COMMAND read was " << str->c_str() << std::endl;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/*  Set the string data to the token
 */
void XCubeSatToken::set_data(Glib::ustring *str)
{
    this->data.sdata = str;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/*  Set the boolean data to the token
 */
void XCubeSatToken::set_data(bool b)
{
    this->data.bdata = b;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/*  Set the integer data to the token
 */
void XCubeSatToken::set_data(int i)
{
    this->data.idata = i;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/*  Get the type of the token
 */
token XCubeSatToken::get_type()
{
    return this->type;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/*  Get the value of the token converted in string
 */
Glib::ustring *XCubeSatToken::get_value_str()
{
    return this->data.sdata;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/*  Get the value of the token converted in int
 */
int XCubeSatToken::get_value_int()
{
    return this->data.idata;
}
/*  --------------------------------------------------------  */
