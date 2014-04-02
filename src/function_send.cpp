#include <vector>
#include <typeinfo>

#include "function_send.hpp"
#include "function_file.hpp"
#include "function_appenddate.hpp"
#include "function_format.hpp"
#include "function_variable_string.hpp"
#include "function_variable_bool.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
FunctionSend::FunctionSend(FunctionVariableString *str, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables):Function(interface, variables)
{
    this->params.push_back(str);

    this->interface = interface;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor: Imitates the form of this function in the
 * definition of the language
 */
FunctionSend::FunctionSend(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*):Function(interface, variables)
{
    if(tokens->empty()) throw new std::exception();
    XCubeSatToken *t = tokens->front();
    
    tokens->erase(tokens->begin());

    Function *f;

    switch(t->get_type()) {
        case FILE2:
            f = new FunctionFile(tokens, this->interface, variables);
            break;
        case APPENDDATE:
            f = new FunctionAppendDate(tokens, this->interface, variables);
            break;
        case FORMAT:
            f = new FunctionFormat(tokens, this->interface, variables);
            break;
        case STRING:
            f = new FunctionVariableString(t->get_value_str());
            break;
        case VARIABLE:
            if(variables->find(*t->get_value_str()) == variables->end()) 
                throw new std::bad_typeid();
            f = (*variables)[*t->get_value_str()];
            if(typeid(*f) != typeid(FunctionVariableString))
                throw new std::bad_typeid();
            break;
        default:
            throw new std::bad_typeid();
    }
    this->params.push_back(f);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Runs the function of the language
 */
Function *FunctionSend::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
std::cout << "SEND(";
    FunctionVariableBool *res = new FunctionVariableBool(true);
    Function *str = this->params[0]->run(runQueue, satName);
    //InOutLog *log = NULL;
std::cout << ")";

    if(str != NULL && typeid(*str) == typeid(FunctionVariableString)) {
        FunctionVariableString *send_str = static_cast<FunctionVariableString*>(str);
std::cout << "{" << *send_str->get_value() << "}";
        if(this->interface->is_configured() && this->interface->is_oppenned())
            this->interface->write(send_str->get_value());
        //    log = this->interface->write(send_str->get_value());
        //if(!log->is_successful())
        else 
            res = new FunctionVariableBool(false);
    }
    return res;
}
/*  --------------------------------------------------------  */
