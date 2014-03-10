#include "function_save.hpp"

FunctionSave::FunctionSave(FunctionVariableData *dat, FunctionVariableString *str)
{
    this->params.push_back(dat);
    this->params.push_back(str);

    this->returnValue = NULL;
}
