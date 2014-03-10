#ifndef FUNCTION_APPENDDATE_HPP
#define FUNCTION_APPENDDATE_HPP

#include "function.hpp"
#include "function_variable_string.hpp"

class FunctionAppendDate : public Function
{
    public:
        FunctionAppendDate(FunctionVariableString *str);
};

#endif
