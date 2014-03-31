#ifndef FUNCTION_VARIABLE_INT_HPP
#define FUNCTION_VARIABLE_INT_HPP

#include "function.hpp"

class FunctionVariableInt : public Function
{
    private:
        int content;
    public:
        FunctionVariableInt();
        FunctionVariableInt(int content);

        void set_value(int i);
        void set_value(FunctionVariableInt*);
        int get_value();

        Function *run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*);
};

#endif
