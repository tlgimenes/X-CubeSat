#ifndef FUNCTION_VARIABLE_BOOL_HPP
#define FUNCTION_VARIABLE_BOOL_HPP

#include "function.hpp"

class FunctionVariableBool : public Function
{
    private:
        bool content;
    public:
        FunctionVariableBool();
        FunctionVariableBool(bool content);

        void set_value(bool content);
        void set_value(FunctionVariableBool*);
        bool get_value();

        Function *run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*);
};

#endif
