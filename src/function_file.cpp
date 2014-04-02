#include <typeinfo>

#include "function_file.hpp"
#include "function_appenddate.hpp"
#include "function_format.hpp"
#include "function_variable_string.hpp"
#include "data_base.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
FunctionFile::FunctionFile(FunctionVariableString *stringName, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables):Function(interface, variables)
{
    this->params.push_back(stringName);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor: Imitates the form of this function in the
 * definition of the language
 */
FunctionFile::FunctionFile(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*):Function(interface, variables)
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
Function *FunctionFile::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    Glib::ustring *data = NULL;
    FunctionVariableString *str = NULL; 

    std::cout << "File(";
    Function *par1 = this->params[0]->run(runQueue, satName);
    std::cout << ")";

    if(par1 != NULL && typeid(*par1) == typeid(FunctionVariableString)) {
        FunctionVariableString *fileName = static_cast<FunctionVariableString*>(par1);
        data = new Glib::ustring(DataBase::get_content_file(*fileName->get_value()));
        str = new FunctionVariableString(data);
    }
    return str;
}
/*  --------------------------------------------------------  */
