#include "function_appenddate.hpp"

FunctionAppendDate::FunctionAppendDate(FunctionVariableString *str)
{
    this->params.push_back(str);

    this->returnValue = NULL;
}
