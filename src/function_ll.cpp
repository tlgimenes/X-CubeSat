#include <typeinfo>

#include "function_ll.hpp"
#include "function_variable_string.hpp"
#include "function_variable_int.hpp"
#include "function_variable_bool.hpp"

FunctionLl::FunctionLl(FunctionVariableInt *int1, FunctionVariableInt *int2, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables):Function(interface, variables)
{
    this->params.push_back(int1);
    this->params.push_back(int2);
}

FunctionLl::FunctionLl(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*):Function(interface, variables)
{
    if(tokens->empty()) throw new std::exception();

    XCubeSatToken *t = tokens->front();

    tokens->erase(tokens->begin());

    Function *f;

    switch(t->get_type()) {
        case STRING:
            f = new FunctionVariableString(t->get_value_str());
            break;
        case VARIABLE:
            if(variables->find(*t->get_value_str()) == variables->end()) 
                throw new std::bad_typeid();
            f = (*variables)[*t->get_value_str()];
            if(typeid(*f) != typeid(FunctionVariableInt))
                throw new std::bad_typeid();
            break;
        default:
            throw new std::bad_typeid();
    }
    this->params.push_back(f);

    t = tokens->front();

    tokens->erase(tokens->begin());

    switch(t->get_type()) {
        case STRING:
            f = new FunctionVariableString(t->get_value_str());
            break;
        case VARIABLE:
            if(variables->find(*t->get_value_str()) == variables->end()) 
                throw new std::bad_typeid();
            f = (*variables)[*t->get_value_str()];
            if(typeid(*f) != typeid(FunctionVariableInt))
                throw new std::bad_typeid();
            break;
        default:
            throw new std::bad_typeid();
    }
    this->params.push_back(f);
}

Function *FunctionLl::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{    
    std::cout << "LL(";
    Function *par1 = this->params[0]->run(runQueue, satName);
    std::cout << ",";
    Function *par2 = this->params[1]->run(runQueue, satName);
    std::cout << ")";

    if(typeid(*par1) == typeid(FunctionVariableInt) && typeid(*par2) == typeid(FunctionVariableInt)) {
       FunctionVariableInt* int1 = static_cast<FunctionVariableInt*>(par1);
       FunctionVariableInt* int2 = static_cast<FunctionVariableInt*>(par2);

       if(int1->get_value() < int2->get_value())
           return new FunctionVariableBool(true);
       else
           return new FunctionVariableBool(false);
    }
    else 
        throw new std::bad_typeid();

    return NULL;
}