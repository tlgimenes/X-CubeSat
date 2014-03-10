#ifndef FUNCTION_DECLARE_HPP
#define FUNCTION_DECLARE_HPP

#include "function.hpp"
#include "function_variable_bool.hpp"
#include "function_variable_string.hpp"

class FunctionDeclare : public Function
{
    public:
        FunctionDeclare(FunctionVariableString *type, FunctionVariableString *name);
};

#endif
