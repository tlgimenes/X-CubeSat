#ifndef FUNCTION_SET_HPP
#define FUNCTION_SET_HPP

#include <unordered_map>

#include "function.hpp"
#include "xcubesat_token.hpp"

class FunctionSet: public Function 
{
    public:
        FunctionSet(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *var) throw(std::bad_typeid*);

        Function *run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*);
};

#endif
