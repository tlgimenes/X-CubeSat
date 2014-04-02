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
    std::cout << "VariableData";

    return this;
}
/*  --------------------------------------------------------  */
