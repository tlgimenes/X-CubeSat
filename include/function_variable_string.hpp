#ifndef FUNCTION_VARIABLE_STRING_HPP
#define FUNCTION_VARIABLE_STRING_HPP

#include <string>
#include <gtkmm.h>

#include "function.hpp"

class FunctionVariableString : public Function
{
    private:
        Glib::ustring *content;
    public:
        FunctionVariableString();
        FunctionVariableString(Glib::ustring *str);

        void set_value(Glib::ustring *str);
        void set_value(FunctionVariableString*);
        Glib::ustring *get_value();

        Function *run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*);
};

#endif
