#include "function_send.hpp"

FunctionSend::FunctionSend(FunctionVariableString *str)
{
    this->params.push_back(str);

    this->returnValue = NULL;
}
