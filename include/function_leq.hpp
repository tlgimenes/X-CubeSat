#ifndef FUNCTION_LEQ_HPP
#define FUNCTION_LEQ_HPP

#include "function.hpp"
#include "function_variable_int.hpp"

class FunctionLeq: public Function
{
    public:
        FunctionLeq(FunctionVariableInt *int1, FunctionVariableInt *int2);
};

#endif
