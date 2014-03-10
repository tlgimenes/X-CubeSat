#include "function_variable_data.hpp"

FunctionVariableData::FunctionVariableData(std::string str)
{
    this->received_data = str;
}

std::string *FunctionVariableData::get_value()
{
    return &(this->received_data);
}
