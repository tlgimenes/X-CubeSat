#include "function_variable_string.hpp"

FunctionVariableString::FunctionVariableString(): Function()
{
    /*
     * Nothing to do here
     */
}

FunctionVariableString::FunctionVariableString(Glib::ustring *str): Function()
{
    this->content = str;
}

void FunctionVariableString::set_value(Glib::ustring *str)
{
    this->content = str;
}

void FunctionVariableString::set_value(FunctionVariableString* d)
{
    this->content = d->get_value();
}

Glib::ustring *FunctionVariableString::get_value()
{
    return this->content;
}

Function *FunctionVariableString::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    std::cout << "VariableString";

    return this;
}
