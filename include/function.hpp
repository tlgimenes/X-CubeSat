#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <vector>
#include <iostream>
#include <string>

class Function 
{
    protected:
        std::vector<Function*> params;
        Function *returnValue;
    public:
        Function();
        Function *get_return();
};

#endif
