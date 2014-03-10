#include "function_file.hpp"

FunctionFile::FunctionFile(FunctionVariableString *stringName)
{
    this->params.push_back(stringName);

    this->returnValue = NULL;
}
