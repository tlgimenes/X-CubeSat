#ifndef FUNCTION_EQ_HPP
#define FUNCTION_EQ_HPP

#include <typeinfo>

#include "function.hpp"
#include "function_variable_bool.hpp"
#include "function_variable_int.hpp"
#include "xcubesat_token.hpp"
#include "in_out_interface.hpp"

class FunctionEq : public Function
{
    public:
        FunctionEq(FunctionVariableInt *, FunctionVariableInt *, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables);
        FunctionEq(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*);

        Function *run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*);
};

#endif
