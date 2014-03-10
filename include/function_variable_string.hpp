#ifndef FUNCTION_VARIABLE_STRING_HPP
#define FUNCTION_VARIABLE_STRING_HPP

#include <string>

#include "function.hpp"

class FunctionVariableString : public Function
{
    private:
        std::string content;
    public:
        FunctionVariableString(std::string str);
        std::string *get_value();
};

#endif
