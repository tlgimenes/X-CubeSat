#include "function_format.hpp"

FunctionFormat::FunctionFormat(FunctionVariableString *str):Function()
{
    this->params.push_back(str);

    this->returnValue = NULL;
}
