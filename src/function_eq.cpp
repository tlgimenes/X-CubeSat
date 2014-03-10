#include "function_eq.hpp"

FunctionEq::FunctionEq(FunctionVariableInt *int1, FunctionVariableInt *int2)
{
    this->params.push_back(int1);
    this->params.push_back(int2);

    this->returnValue = NULL;
}
