#include "function_variable_bool.hpp"

FunctionVariableBool::FunctionVariableBool():Function()
{
    this->content = false;
}

FunctionVariableBool::FunctionVariableBool(bool content):Function()
{
    this->content = content;
}

void FunctionVariableBool::set_value(bool content)
{
    this->content = content;
}

void FunctionVariableBool::set_value(FunctionVariableBool* d)
{
    this->content = d->get_value();
}

bool FunctionVariableBool::get_value()
{
    return this->content;
}

Function *FunctionVariableBool::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    std::cout << "VariableBool";

    return this;
}
