#include <typeinfo>

#include "function_declare.hpp"
#include "function_file.hpp"
#include "function_appenddate.hpp"
#include "function_format.hpp"
#include "function_variable.hpp"
#include "function_variable_bool.hpp"
#include "function_variable_data.hpp"
#include "function_variable_int.hpp"
#include "function_variable_string.hpp"

FunctionDeclare::FunctionDeclare(FunctionVariableString *type, FunctionVariableString *name, InOutInterface *interface, std::unordered_map<std::string, Function*> *var):Function(interface, variables)
{
    this->params.push_back(type);
    this->params.push_back(name);
}

FunctionDeclare::FunctionDeclare(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *var) throw(std::bad_typeid*):Function(interface, variables)
{
    if(tokens->empty()) throw new std::exception();

    XCubeSatToken *t = tokens->front();

    tokens->erase(tokens->begin());

    Function *f;
    Glib::ustring *type;

    switch(t->get_type()) {
        case STRING:
        case VARIABLE:
            type = t->get_value_str();
            type->uppercase();
            if(!type->compare("BOOL")) {
                f = new FunctionVariableBool(false);}
            else if(!type->compare("DATA")) {
                f = new FunctionVariableData(NULL, NULL, 0);}
            else if(!type->compare("INT")) {
                f = new FunctionVariableInt(0);}
            else if(!type->compare("STRING")) {
                f = new FunctionVariableString(NULL); }
            else {
                throw new std::bad_typeid();}
            break;
        default:
            throw new std::bad_typeid();
    }
    this->params.push_back(f);

    t = tokens->front();
    tokens->erase(tokens->begin());

    std::pair<std::string, Function*> *p;
    switch(t->get_type()) {
        case VARIABLE:
            p = new std::pair<std::string, Function*>(*t->get_value_str(), f);
            var->insert(*p);
            break;
        default:
            throw new std::bad_typeid();
    }
}

Function *FunctionDeclare::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    std::cout << "Declare(NULL)";

    return NULL;
}
