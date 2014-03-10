#include "function.hpp"
#include "log.hpp"

Function::Function()
{
    this->returnValue = NULL;
}

Function *Function::get_return()
{
    return this->returnValue;
}
