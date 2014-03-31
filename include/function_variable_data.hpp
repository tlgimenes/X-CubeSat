#ifndef FUNCTION_VARIABLE_DATA_HPP
#define FUNCTION_VARIABLE_DATA_HPP

#include <gtkmm.h>

#include "function.hpp"

class FunctionVariableData : public Function
{
    private:
        Glib::ustring received_data;
        Glib::ustring format;
        unsigned int date;

    public:
        FunctionVariableData();
        FunctionVariableData(Glib::ustring str, Glib::ustring format, unsigned int date);
        FunctionVariableData(FunctionVariableData*);

        void set_value(Glib::ustring str, Glib::ustring format, unsigned int date);
        void set_value(FunctionVariableData*);
        Glib::ustring *get_value_received();
        Glib::ustring *get_value_format();
        unsigned int get_value_date();

        Function *run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*);
};

#endif
