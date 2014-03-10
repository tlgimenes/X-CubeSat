#ifndef FUNCTION_IF_HPP
#define FUNCTION_IF_HPP

#include "function.hpp"
#include "function_variable_bool.hpp"

class FunctionIf : public Function
{
    public:
        FunctionIf(FunctionVariableBool *boolean);
};

#endif
