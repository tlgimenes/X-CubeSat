#ifndef FUNCTION_RECEIVE_HPP
#define FUNCTION_RECEIVE_HPP

#include "function.hpp"
#include "function_variable_string.hpp"

class FunctionReceive : public Function
{
    public:
        FunctionReceive(FunctionVariableString *str);
};

#endif
