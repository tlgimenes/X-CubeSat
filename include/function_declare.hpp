#ifndef FUNCTION_DECLARE_HPP
#define FUNCTION_DECLARE_HPP

#include <typeinfo>
#include <unordered_map>

#include "function.hpp"
#include "function_variable_bool.hpp"
#include "function_variable_string.hpp"
#include "xcubesat_token.hpp"
#include "in_out_interface.hpp"

class FunctionDeclare : public Function
{
    public:
        FunctionDeclare(FunctionVariableString *type, FunctionVariableString *name, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables);
        FunctionDeclare(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*);

        Function *run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*);
};

#endif
