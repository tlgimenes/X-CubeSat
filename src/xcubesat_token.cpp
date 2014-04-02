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
    std::cout << "the string read was " << str->c_str() << std::endl;
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
