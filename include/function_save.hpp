#ifndef FUNCTION_SAVE_HPP
#define FUNCTION_SAVE_HPP

#include "function.hpp"
#include "function_variable_string.hpp"
#include "function_variable_data.hpp"
#include "xcubesat_token.hpp"
#include "in_out_interface.hpp"

class FunctionSave : public Function
{
    public:
        FunctionSave(FunctionVariableData *dat, FunctionVariableString *str, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables);
        FunctionSave(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*);

        Function *run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*);
};

#endif
