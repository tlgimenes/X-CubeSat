#ifndef FUNCTION_SEND_HPP
#define FUNCTION_SEND_HPP

#include "function.hpp"
#include "function_variable_string.hpp"

class FunctionSend : public Function
{
    public:
        FunctionSend(FunctionVariableString *str);
};

#endif
