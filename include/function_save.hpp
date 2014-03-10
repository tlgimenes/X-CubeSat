#ifndef FUNCTION_SAVE_HPP
#define FUNCTION_SAVE_HPP

#include "function.hpp"
#include "function_variable_string.hpp"
#include "function_variable_data.hpp"

class FunctionSave : public Function
{
    public:
        FunctionSave(FunctionVariableData *dat, FunctionVariableString *str);
};

#endif
