#ifndef FUNCTION_IF_HPP
#define FUNCTION_IF_HPP

#include <typeinfo>

#include "function.hpp"
#include "function_variable_bool.hpp"
#include "xcubesat_token.hpp"
#include "in_out_interface.hpp"

class FunctionIf : public Function
{
    public:
        FunctionIf(FunctionVariableBool *boolean, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables);
        FunctionIf(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*);

        Function *run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*);
};

#endif
