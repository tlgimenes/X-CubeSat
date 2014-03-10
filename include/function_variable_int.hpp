#ifndef FUNCTION_VARIABLE_INT_HPP
#define FUNCTION_VARIABLE_INT_HPP

#include "function.hpp"

class FunctionVariableInt : public Function
{
    private:
        int content;
    public:
        FunctionVariableInt(int content);
        int *get_value();
};

#endif
