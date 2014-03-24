#ifndef FUNCTION_VARIABLE_HPP
#define FUNCTION_VARIABLE_HPP

#include "function.hpp"

typedef enum var_t {
    BOOL_T,
    DATA_T,
    INT_T,
    STRING_T
} var_t;

class FunctionVariable : public Function
{
    protected:
        Glib::ustring name;

    public:
        virtual void set_value();
        virtual void *get_value();
};

#endif
