#ifndef FUNCTION_LEQ_HPP
#define FUNCTION_LEQ_HPP

#include "function.hpp"
#include "function_variable_int.hpp"
#include "xcubesat_token.hpp"
#include "in_out_interface.hpp"

class FunctionLeq: public Function
{
    public:
        FunctionLeq(FunctionVariableInt *int1, FunctionVariableInt *int2, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables);
        FunctionLeq(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*);

        Function *run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*);
};

#endif
