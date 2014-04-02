#ifndef XCUBESAT_COMPILER_HPP
#define XCUBESAT_COMPILER_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <gtkmm.h>
#include <typeinfo>

#include "function.hpp"
#include "xcubesat_token.hpp"
#include "in_out_interface.hpp"

class XCubeSatCompiler 
{
    private:
        std::vector<XCubeSatToken*> *tokens;
        Glib::ustring log;
        bool error;

    protected:
        std::vector<XCubeSatToken*> *tokenizer(std::stringstream *fileString);
        std::stringstream *replace_alias(std::unordered_map<std::string, std::string> *alias, std::stringstream *fileString);

    public:
        XCubeSatCompiler();
        
        std::vector<Function*> *compile(std::unordered_map<std::string, std::string> *alias, std::stringstream *fileString, InOutInterface *interface) throw(std::bad_typeid*);

        bool are_there_syntax_errors();
        Glib::ustring get_syntax_errors();
};

#endif
