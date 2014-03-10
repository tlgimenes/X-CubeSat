#include "function_variable_string.hpp"

FunctionVariableString::FunctionVariableString(std::string str)
{
    this->content = str;
}

std::string *FunctionVariableString::get_value()
{
    return &(this->content);
}
