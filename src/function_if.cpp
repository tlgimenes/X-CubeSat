#include <iostream>
#include <typeinfo>

#include "xcubesat_compiler.hpp"
#include "function_send.hpp"
#include "function_save.hpp"
#include "function_if.hpp"
#include "function_declare.hpp"
#include "function_eq.hpp"
#include "function_ll.hpp"
#include "function_leq.hpp"
#include "function_file.hpp"
#include "function_appenddate.hpp"
#include "function_format.hpp"
#include "function_receive.hpp"
#include "function_variable_string.hpp"
#include "function_variable_bool.hpp"
#include "function_set.hpp"
#include "function_declare.hpp"

FunctionIf::FunctionIf(FunctionVariableBool *boolean, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables):Function(interface, variables)
{
    this->params.push_back(boolean);
}

FunctionIf::FunctionIf(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*):Function(interface, variables)
{
    if(tokens->empty()) throw new std::exception();

    XCubeSatToken *t = tokens->front();

    tokens->erase(tokens->begin());

    Function *f;

    switch(t->get_type()) {
        case SEND:
            f = new FunctionSend(tokens, this->interface, variables);
            break;
        case SAVE:
            f = new FunctionSave(tokens, this->interface, variables);
            break;
        case IF:
            f = new FunctionIf(tokens, this->interface, variables);
            break;
        case EQ:
        case EQUALS:
            f = new FunctionEq(tokens, this->interface, variables);
            break;
        case LL:
            f = new FunctionLl(tokens, this->interface, variables);
            break;
        case LEQ:
            f = new FunctionLeq(tokens, this->interface, variables);
            break;
        case VARIABLE:
            if(variables->find(*t->get_value_str()) == variables->end()) 
                throw new std::bad_typeid();
            f = (*variables)[*t->get_value_str()];
            if(typeid(*f) != typeid(FunctionVariableBool))
                throw new std::bad_typeid();
            break;
        default:
            throw new std::bad_typeid();
    }
    this->params.push_back(f);

    t = tokens->front();
    tokens->erase(tokens->begin());

    if(t->get_type() != LPAREN) throw new std::bad_typeid();

    while(!tokens->empty() && tokens->front()->get_type() != RPAREN) {
        t = tokens->front();

        tokens->erase(tokens->begin());

        switch(t->get_type()) {
            case SEND:
                f = new FunctionSend(tokens, this->interface, variables);
                break;
            case SAVE:
                f = new FunctionSave(tokens, this->interface, variables);
                break;
            case IF:
                f = new FunctionIf(tokens, this->interface, variables);
                break;
            case EQ:
                f = new FunctionEq(tokens, this->interface, variables);
                break;
            case LL:
                f = new FunctionLl(tokens, this->interface, variables);
                break;
            case LEQ:
                f = new FunctionLeq(tokens, this->interface, variables);
                break;
            case FILE2:
                f = new FunctionFile(tokens, this->interface, variables);
                break;
            case APPENDDATE:
                f = new FunctionAppendDate(tokens, this->interface, variables);
                break;
            case FORMAT:
                f = new FunctionFormat(tokens, this->interface, variables);
                break;
            case RECEIVE:
                f = new FunctionReceive(tokens, this->interface, variables);
                break;
            case SET:
                f = new FunctionSet(tokens, this->interface, variables);
                break;
            case DECLARE:
                f = new FunctionDeclare(tokens, this->interface, variables);
                break;
            case EQUALS:
                /* TODO */
                break;
            case STRING:
 /*               f = new FunctionVariableString(t->get_value_str());
                break;
            case VARIABLE:
                if(variables->find(*t->get_value_str()) == variables->end()) 
                    throw new std::bad_typeid();
                f = (*variables)[*t->get_value_str()];
                break;*/
            default:
                throw new std::bad_typeid();
        }
        this->params.push_back(f);
    }
}

Function *FunctionIf::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    bool res;

std::cout << "IF(";
    Function *f = this->params[0]->run(runQueue, satName);
std::cout << ")";

    if(f != NULL && typeid(*f) == typeid(FunctionVariableBool)) {
        FunctionVariableBool *f2 = static_cast<FunctionVariableBool*>(f);
        res = f2->get_value();
        if(res && this->params.size() > 1) {
std::cout << std::endl << "{" << std::endl;
            for(unsigned int i=1; i < this->params.size(); i++) {
                this->params[i]->run(runQueue, satName);
std::cout << std::endl;
            }
std::cout << "}" << std::endl;
        }
    }

    return new FunctionVariableBool(true);
}
