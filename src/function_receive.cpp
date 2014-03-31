#include <typeinfo>
#include <ctime>

#include "function_receive.hpp"
#include "function_file.hpp"
#include "function_appenddate.hpp"
#include "function_format.hpp"
#include "function_variable_string.hpp"
#include "function_variable_data.hpp"

FunctionReceive::FunctionReceive(FunctionVariableString *str, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables):Function(interface, variables)
{
    this->params.push_back(str);
}

FunctionReceive::FunctionReceive(std::vector<XCubeSatToken*> *tokens, InOutInterface *interface, std::unordered_map<std::string, Function*> *variables) throw(std::bad_typeid*):Function(interface, variables)
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

Function *FunctionReceive::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    FunctionVariableData *data = NULL;
    InOutLog *log;
    time_t raw_time;

    time(&raw_time);
    struct tm *ptm = gmtime(&raw_time);

    std::cout << "Receive(";
    Function *format = this->params[0]->run(runQueue, satName);
    std::cout << ")";

    if(format != NULL && typeid(*format) == typeid(FunctionVariableString)) {
        FunctionVariableString *format_str = static_cast<FunctionVariableString*>(format);

        if(this->interface->is_oppenned() && this->interface->is_configured()) {
            log = this->interface->read('\n');
            if(log->is_successful()) {
                data = new FunctionVariableData(*log->get_data(), *format_str->get_value(), (ptm->tm_hour)*10000 + ptm->tm_min*100 + ptm->tm_sec);
            }
        }
    }

    return data;
}
