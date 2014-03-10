#ifndef FUNCTION_FILE_HPP
#define FUNCTION_FILE_HPP

#include "function.hpp"
#include "function_variable_string.hpp"

class FunctionFile : public Function
{
    public:
        FunctionFile(FunctionVariableString *);
};

#endif
