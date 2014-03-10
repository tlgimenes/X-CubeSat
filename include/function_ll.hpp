#ifndef FUNCTION_LL_HPP
#define FUNCTION_LL_HPP

#include "function.hpp"
#include "function_variable_int.hpp"

class FunctionLl : public Function
{
    public:
        FunctionLl(FunctionVariableInt *int1, FunctionVariableInt *int2);
};

#endif
