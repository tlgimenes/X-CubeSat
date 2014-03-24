#ifndef FUNCTION_SEND_HPP
#define FUNCTION_SEND_HPP

#include "function.hpp"
#include "function_variable_string.hpp"
#include "xcubesat_token.hpp"
#include "in_out_interface.hpp"

class FunctionSend : public Function
{
    public:
        FunctionSend(FunctionVariableString *str, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables);
        FunctionSend(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*);

        Function *run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*);
};

#endif
