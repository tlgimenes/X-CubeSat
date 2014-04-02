#include "function_variable_string.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
FunctionVariableString::FunctionVariableString(): Function()
{
    /*
     * Nothing to do here
     */
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor
 */
FunctionVariableString::FunctionVariableString(Glib::ustring *str): Function()
{
    this->content = str;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Sets the value of this variable
 */
void FunctionVariableString::set_value(Glib::ustring *str)
{
    this->content = str;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Sets the value of this variable
 */
void FunctionVariableString::set_value(FunctionVariableString* d)
{
    this->content = d->get_value();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the value of this variable
 */
Glib::ustring *FunctionVariableString::get_value()
{
    return this->content;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Runs the function of the language
 */
Function *FunctionVariableString::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    std::cout << "VariableString";

    return this;
}
/*  --------------------------------------------------------  */
