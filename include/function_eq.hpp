#ifndef FUNCTION_EQ_HPP
#define FUNCTION_EQ_HPP

#include "function.hpp"
#include "function_variable_bool.hpp"
#include "function_variable_int.hpp"

class FunctionEq : public Function
{
    public:
        FunctionEq(FunctionVariableInt *, FunctionVariableInt *);
};

#endif
