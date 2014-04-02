#include <typeinfo>
#include <ctime>

#include "function_appenddate.hpp"
#include "function_file.hpp"
#include "function_format.hpp"
#include "function_receive.hpp"
#include "function_variable_string.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
FunctionAppendDate::FunctionAppendDate(FunctionVariableString *str, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables):Function(interface, variables)
{
    this->params.push_back(str);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor: Imitates the form of this function in the
 * definition of the language
 */
FunctionAppendDate::FunctionAppendDate(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*):Function(interface, variables)
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
Function *FunctionAppendDate::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    FunctionVariableString *res = NULL;
    Glib::ustring date;
    Glib::ustring *aux;
    time_t raw_time;

    std::cout << "AppendDate(";
    Function *par1 = this->params[0]->run(runQueue, satName);
    std::cout << ")";

    if(par1 != NULL && typeid(*par1) == typeid(FunctionVariableString)) {
        FunctionVariableString *str = static_cast<FunctionVariableString*>(par1);
        aux = str->get_value();

        time(&raw_time);
        date = asctime(gmtime(&raw_time));

        aux->append(date);

        res = new FunctionVariableString(aux);
    }
    return res;
}
/*  --------------------------------------------------------  */
