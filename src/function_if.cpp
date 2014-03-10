#include <iostream>

#include "function_if.hpp"

FunctionIf::FunctionIf(FunctionVariableBool *boolean)
{
    this->params.push_back(boolean);

    this->returnValue = NULL;
}
