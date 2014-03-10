#include "function_ll.hpp"

FunctionLl::FunctionLl(FunctionVariableInt *int1, FunctionVariableInt *int2)
{
    this->params.push_back(int1);
    this->params.push_back(int2);

    this->returnValue = NULL;
}
