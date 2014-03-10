#ifndef FUNCTION_VARIABLE_DATA_HPP
#define FUNCTION_VARIABLE_DATA_HPP

#include <string>

#include "function.hpp"

class FunctionVariableData : public Function
{
    private:
        std::string received_data;
    public:
        FunctionVariableData(std::string str);
        std::string *get_value();
};

#endif
