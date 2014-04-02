#include "function_variable_bool.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
FunctionVariableBool::FunctionVariableBool():Function()
{
    this->content = false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor
 */
FunctionVariableBool::FunctionVariableBool(bool content):Function()
{
    this->content = content;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Sets the value of this variable
 */
void FunctionVariableBool::set_value(bool content)
{
    this->content = content;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Sets the value of this variable
 */
void FunctionVariableBool::set_value(FunctionVariableBool* d)
{
    this->content = d->get_value();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the value of this variable
 */
bool FunctionVariableBool::get_value()
{
    return this->content;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Runs the function of the language
 */
Function *FunctionVariableBool::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    std::cout << "VariableBool";

    return this;
}
/*  --------------------------------------------------------  */
