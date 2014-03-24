#ifndef FUNCTION_APPENDDATE_HPP
#define FUNCTION_APPENDDATE_HPP

#include <typeinfo>

#include "function.hpp"
#include "function_variable_string.hpp"
#include "xcubesat_token.hpp"
#include "in_out_interface.hpp"

class FunctionAppendDate : public Function
{
    public:
        FunctionAppendDate(FunctionVariableString *str, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables);
        FunctionAppendDate(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*);

        Function *run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*);
};

#endif
