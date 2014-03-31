#ifndef FUNCTION_LL_HPP
#define FUNCTION_LL_HPP

#include "function.hpp"
#include "function_variable_int.hpp"
#include "xcubesat_token.hpp"
#include "in_out_interface.hpp"

class FunctionLl : public Function
{
    public:
        FunctionLl(FunctionVariableInt *int1, FunctionVariableInt *int2, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables);
        FunctionLl(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*);

        Function *run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*);
};

#endif
