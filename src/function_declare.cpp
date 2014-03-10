#include "function_declare.hpp"

FunctionDeclare::FunctionDeclare(FunctionVariableString *type, FunctionVariableString *name):Function()
{
    this->params.push_back(type);
    this->params.push_back(name);

    this->returnValue = NULL;
}
