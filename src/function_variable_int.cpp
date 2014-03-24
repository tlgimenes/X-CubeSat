#include "function_variable_int.hpp"

FunctionVariableInt::FunctionVariableInt(): Function()
{
    this->content = 0;
}

FunctionVariableInt::FunctionVariableInt(int content): Function()
{
    this->content = content;
}

void FunctionVariableInt::set_value(int i)
{
    this->content = i;
}

void FunctionVariableInt::set_value(FunctionVariableInt* d)
{
    this->content = d->get_value();
}

int FunctionVariableInt::get_value()
{
    return this->content;
}

Function *FunctionVariableInt::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    std::cout << "VariableInt";

    return this;
}
