#include "function_variable_bool.hpp"

FunctionVariableBool::FunctionVariableBool(bool content)
{
    this->content = content;
}

bool *FunctionVariableBool::get_value()
{
    return &(this->content);
}
