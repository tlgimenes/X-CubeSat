#include "function_variable_int.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
FunctionVariableInt::FunctionVariableInt(): Function()
{
    this->content = 0;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor
 */
FunctionVariableInt::FunctionVariableInt(int content): Function()
{
    this->content = content;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Sets the value of this variable
 */
void FunctionVariableInt::set_value(int i)
{
    this->content = i;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Sets the value of this variable
 */
void FunctionVariableInt::set_value(FunctionVariableInt* d)
{
    this->content = d->get_value();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the value of this variable
 */
int FunctionVariableInt::get_value()
{
    return this->content;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Runs the function of the language
 */
Function *FunctionVariableInt::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    std::cout << "VariableInt";

    return this;
}
/*  --------------------------------------------------------  */
