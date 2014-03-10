#include "function_variable_int.hpp"

FunctionVariableInt::FunctionVariableInt(int content)
{
    this->content = content;
}

int *FunctionVariableInt::get_value()
{
    return &(this->content);
}
