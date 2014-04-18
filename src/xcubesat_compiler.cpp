/* X-CubeSat Controler: Real-time communication with satellite program

 Copyright (C)  2014 - Tiago Lobato Gimenes

 Authors: Tiago Lobato Gimenes <tlgimenes@gmail.com>

 Comments, questions and bugreports should be submitted via
 https://github.com/tlgimenes/X-CubeSat
 More details can be found at the project home page:

 https://github.com/tlgimenes/X-CubeSat

 This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

 This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
along with this program; if not, visit http://www.fsf.org/
*/

#include <stack>
#include <typeinfo>
#include <unordered_map>

#include "xcubesat_compiler.hpp"
#include "function_send.hpp"
#include "function_save.hpp"
#include "function_if.hpp"
#include "function_declare.hpp"
#include "function_set.hpp"
#include "function_eq.hpp"
#include "function_ll.hpp"
#include "function_leq.hpp"
#include "function_file.hpp"
#include "function_appenddate.hpp"
#include "function_format.hpp"
#include "function_receive.hpp"
#include "function_variable_string.hpp"
#include "defs.hpp"

/*  --------------------------------------------------------  */
/* Constructors
 */
XCubeSatCompiler::XCubeSatCompiler()
{
    this->tokens = NULL;
    this->error = false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Gets a raw string with its alias and a valid Input/Output
 * term and returns a vector of Function to be executed.
 * If all goes well it is just necessary to do a foreach in
 * the return structure asking for the method run
 */
std::vector<Function*> *XCubeSatCompiler::compile(std::unordered_map<std::string, std::string> *alias, std::stringstream *fileString, Terminal *term) throw(std::bad_typeid*)
{
    std::unordered_map<std::string, Function*> variables;
    std::vector<Function*> *vec = new std::vector<Function*>();
    XCubeSatToken *t;
    Function *f;

    std::stringstream *fileStringNoAlias = this->replace_alias(alias, fileString);
    this->tokens = this->tokenizer(fileStringNoAlias);

    // At this point tokens should be ok (they 
    // should respect the language definition)
    while(!this->tokens->empty()) {
        t = this->tokens->front();
        this->tokens->erase(this->tokens->begin());
        switch(t->get_type()) {
            case SEND:
                f = new FunctionSend(this->tokens, term, &variables);
                break;
            case SAVE:
                f = new FunctionSave(this->tokens, term, &variables);
                break;
            case IF:
                f = new FunctionIf(this->tokens, term, &variables);
                break;
            case EQ:
                f = new FunctionEq(this->tokens, term, &variables);
                break;
            case LL:
                f = new FunctionLl(this->tokens, term, &variables);
                break;
            case LEQ:
                f = new FunctionLeq(this->tokens, term, &variables);
                break;
            case FILE2:
                f = new FunctionFile(this->tokens, term, &variables);
                break;
            case APPENDDATE:
                f = new FunctionAppendDate(this->tokens, term, &variables);
                break;
            case FORMAT:
                f = new FunctionFormat(this->tokens, term, &variables);
                break;
            case RECEIVE:
                f = new FunctionReceive(this->tokens, term, &variables);
                break;
            case EQUALS:
                /* TODO */
                break;
            case STRING:
                if(t->get_value_str()->size() <= 1) /* String can not be empty */
                    continue;
                f = new FunctionVariableString(t->get_value_str());
                break;
            case DECLARE:
                f = new FunctionDeclare(this->tokens, term, &variables);
                break;
            case SET:
                f = new FunctionSet(this->tokens, term, &variables);
                break;
            case VARIABLE:
                if(variables.find(*t->get_value_str()) == variables.end()) 
                    throw new std::bad_typeid();
                f = variables[*t->get_value_str()];
                break;
            case RPAREN:
            case LPAREN:
                continue;
            default:
                throw new std::bad_typeid();
        }
        vec->push_back(f);
    }
    return vec;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Discard space, newline and tab characters of a stringstream
 */
#define discard_blanks(sstm) \
    while(!sstm->eof()) { \
        entered = false; \
        while(sstm->peek() == ' ') { \
            sstm->seekg((int)sstm->tellg() + 1); \
            entered = true; \
        } \
        while(sstm->peek() == '\n') { \
            sstm->seekg((int)sstm->tellg() + 1); \
            entered = true; \
        } \
        while(sstm->peek() == 9) { \
            sstm->seekg((int)sstm->tellg() + 1); \
            entered = true; \
        } \
        while(sstm->peek() == 11) { \
            sstm->seekg((int)sstm->tellg() + 1); \
            entered = true; \
        } \
        if(!entered){ \
            break;\
        }\
    }
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Gets the type represented by the string str
 */
#define get_type(str) \
    if(!str.compare("SEND")) \
        t = SEND; \
    else if(!str.compare("RECEIVE")) \
        t = RECEIVE; \
    else if(!str.compare("SAVE")) \
        t = SAVE; \
    else if(!str.compare("IF")) \
        t = IF; \
    else if(!str.compare("DECLARE")) \
        t = DECLARE; \
    else if(!str.compare("EQ")) \
        t = EQ; \
    else if(!str.compare("LL")) \
        t = LL; \
    else if(!str.compare("LEQ")) \
        t = LEQ; \
    else if(!str.compare("FILE")) \
        t = FILE2; \
    else if(!str.compare("APPENDDATE")) \
        t = APPENDDATE; \
    else if(!str.compare("FORMAT")) \
        t = FORMAT; \
    else if(!str.compare("RECEIVE")) \
        t = RECEIVE; \
    else \
        t = NULL2;
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Generates tokens from the string fileString. 
 * This function also generates errors dialogs if the string
 * fileString contains syntax errors.
 */
std::vector<XCubeSatToken*> *XCubeSatCompiler::tokenizer(std::stringstream *fileString)
{
    char *garbage = new char[MAX_COMMENTS_SIZE];
    char c;
    bool entered = false;
    bool insideStr = false;
    Glib::ustring buffer;
    std::stack<char> paren;
    std::stack<char> quotes;
    XCubeSatToken *t;

    this->tokens = new std::vector<XCubeSatToken*>();
    this->error = false;

    discard_blanks(fileString);
    while(!fileString->eof() && !error) {
        c = fileString->get();
        switch(c) {
            case '(':
            case ')':
            case '{':
            case '}':
            case '=':
            case ',':
                if(insideStr) break;
                if(!buffer.empty()) {
                    t = new XCubeSatToken(&buffer);
                    this->tokens->push_back(t);
                    buffer.clear();
                }
        }
        switch(c) {
            case '(':
                paren.push(c);
                discard_blanks(fileString);
                continue;
            case '{':
                buffer.push_back(c);
                t = new XCubeSatToken(&buffer);
                this->tokens->push_back(t);
                buffer.clear();
                paren.push(c);
                discard_blanks(fileString);
                continue;
            case '}':
                buffer.push_back(c);
                t = new XCubeSatToken(&buffer);
                this->tokens->push_back(t);
                buffer.clear();
                if(!paren.empty()) 
                    paren.pop();
                else 
                    error = true;
                discard_blanks(fileString);
                continue;
            case ')':
                if(!paren.empty()) 
                    paren.pop();
                else 
                    error = true;
                discard_blanks(fileString);
                continue;
            case '=':
            case ',':
                discard_blanks(fileString);
                continue;
            case '"':
                insideStr = false;
                if(!buffer.empty()) {
                    t = new XCubeSatToken(&buffer);
                    this->tokens->push_back(t);
                    buffer.clear();
                }
                if(quotes.empty()) {
                    quotes.push(c);
                    insideStr = true;
                    c = toupper(c);
                    buffer.push_back(c);
                }
                else {
                    quotes.pop();
                    insideStr = false;
                }
                discard_blanks(fileString);
                continue;
            case '#':
                fileString->getline(garbage, MAX_COMMENTS_SIZE);
                discard_blanks(fileString);
                continue;

        }
        c = toupper(c);
        buffer.push_back(c);

        if(!insideStr)
            discard_blanks(fileString);
    }
    if(!buffer.empty()) {
        t = new XCubeSatToken(&buffer);
        this->tokens->push_back(t);
    }
    if(!quotes.empty() || !paren.empty() || error) {
        this->log = "Unmatched number of parenthesis and string signal";
        this->error = true;
    }

    return this->tokens;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Gets a string fileString and replaces each word that has
 * a corresponding alias
 */
std::stringstream *XCubeSatCompiler::replace_alias(std::unordered_map<std::string, std::string> *alias, std::stringstream *fileString)
{ 
    char *garbage = new char[MAX_COMMENTS_SIZE];
    char c;
    bool entered = false;
    bool insideStr = false;
    Glib::ustring buffer;
    std::stringstream *str = new std::stringstream();
    std::stack<char> paren;
    std::stack<char> quotes;

    while(!fileString->eof() && !error) {
        c = fileString->get();
        switch(c) {
            case '(':
            case ')':
            case '{':
            case '}':
            case '=':
            case ',':
                if(insideStr) break;
                if(!buffer.empty()) {
                    if(alias->find(buffer.c_str()) != alias->end()) {
                        *str << (*alias)[buffer.c_str()];
                    }
                    else {
                        *str << buffer;
                    }
                    buffer.clear();
                }
                *str << c;
                break;
        }

        switch(c) {
            case '(':
                paren.push(c);
                discard_blanks(fileString);
                continue;
            case '{':
                paren.push(c);
                discard_blanks(fileString);
                continue;
            case '}':
                if(!paren.empty()) 
                    paren.pop();
                else 
                    error = true;
                discard_blanks(fileString);
                continue;
            case ')':
                if(!paren.empty()) 
                    paren.pop();
                else 
                    error = true;
                discard_blanks(fileString);
                continue;
            case '=':
            case ',':
                discard_blanks(fileString);
                continue;
            case '"':
                insideStr = false;
                if(!buffer.empty()) {
                    if(alias->find(buffer.c_str()) != alias->end()) {
                        *str << (*alias)[buffer.c_str()];
                    }
                    else {
                        *str << buffer;
                    }
                    buffer.clear();
                }
                *str << c;
                if(quotes.empty()) {
                    quotes.push(c);
                    insideStr = true;
                }
                else {
                    quotes.pop();
                    insideStr = false;
                }
                discard_blanks(fileString);
                continue;
            case '#':
                fileString->getline(garbage, MAX_COMMENTS_SIZE);
                discard_blanks(fileString);
                continue;

        }
        c = toupper(c);
        buffer.push_back(c);

        if(!insideStr)
            discard_blanks(fileString);
    }
    if(!quotes.empty() || !paren.empty() || error) {
        this->log = "Unmatched number of parenthesis and string signal";
        this->error = true;
    }

    return str;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Check for the flag of syntax_errors
 */
bool XCubeSatCompiler::are_there_syntax_errors()
{
    if(this->error)
        return true;
    return false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Get the string with syntax errors
 */
Glib::ustring XCubeSatCompiler::get_syntax_errors()
{
    return this->log;
}
/*  --------------------------------------------------------  */
