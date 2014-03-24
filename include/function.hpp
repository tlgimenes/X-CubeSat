#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <vector>
#include <iostream>
#include <string>
#include <typeinfo>
#include <unordered_map>

#include "in_out_interface.hpp"

class Function 
{
    protected:
        std::vector<Function*> params;
        InOutInterface *interface;
        std::unordered_map<std::string, Function*> *variables;

    public:
        Function();
        Function(InOutInterface *interface, std::unordered_map<std::string, Function*> *variables);
        virtual Function *run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*) {return NULL;}
};

#endif
