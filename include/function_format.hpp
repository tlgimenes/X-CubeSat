#ifndef FUNCTION_FORMAT_HPP
#define FUNCTION_FORMAT_HPP

#include "function.hpp"
#include "function_variable_string.hpp"

class FunctionFormat : public Function
{
    public:
        FunctionFormat(FunctionVariableString *str);
};

#endif
