#ifndef FUNCTION_VARIABLE_BOOL_HPP
#define FUNCTION_VARIABLE_BOOL_HPP

#include "function.hpp"

class FunctionVariableBool : public Function
{
    private:
        bool content;
    public:
        FunctionVariableBool(bool content);
        bool *get_value();
};

#endif
