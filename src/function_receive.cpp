#include "function_receive.hpp"

FunctionReceive::FunctionReceive(FunctionVariableString *str)
{
    this->params.push_back(str);

    this->returnValue = NULL;
}
