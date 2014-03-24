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

XCubeSatCompiler::XCubeSatCompiler()
{
    this->alias = NULL;
    this->fileString = NULL;
    this->tokens = NULL;
    this->error = false;
}

std::vector<Function*> *XCubeSatCompiler::compile(std::unordered_map<std::string, std::string> *alias, std::stringstream *fileString, InOutInterface *interface) throw(std::bad_typeid*)
{
    std::unordered_map<std::string, Function*> variables;
    std::vector<Function*> *vec = new std::vector<Function*>();
    XCubeSatToken *t;
    Function *f;

    this->alias = alias;
    this->fileString = fileString;

    this->tokens = this->tokenizer();

    // At this point tokens should be ok (they 
    // should respect the language definition)
    while(!this->tokens->empty()) {
        t = this->tokens->front();
        this->tokens->erase(this->tokens->begin());
        switch(t->get_type()) {
            case SEND:
                f = new FunctionSend(this->tokens, interface, &variables);
                break;
            case SAVE:
                f = new FunctionSave(this->tokens, interface, &variables);
                break;
            case IF:
                f = new FunctionIf(this->tokens, interface, &variables);
                break;
            case EQ:
                f = new FunctionEq(this->tokens, interface, &variables);
                break;
            case LL:
                f = new FunctionLl(this->tokens, interface, &variables);
                break;
            case LEQ:
                f = new FunctionLeq(this->tokens, interface, &variables);
                break;
            case FILE2:
                f = new FunctionFile(this->tokens, interface, &variables);
                break;
            case APPENDDATE:
                f = new FunctionAppendDate(this->tokens, interface, &variables);
                break;
            case FORMAT:
                f = new FunctionFormat(this->tokens, interface, &variables);
                break;
            case RECEIVE:
                f = new FunctionReceive(this->tokens, interface, &variables);
                break;
            case EQUALS:
                /* TODO */
                break;
            case STRING:
                f = new FunctionVariableString(t->get_value_str());
                break;
            case DECLARE:
                f = new FunctionDeclare(this->tokens, interface, &variables);
                break;
            case SET:
                f = new FunctionSet(this->tokens, interface, &variables);
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

/*#define discard_paren(sstm) \
    while(!sstm->eof()) { \
        entered = false; \
        while(sstm->peek() == '(') { \
            sstm->seekg((int)sstm->tellg() + 1); \
            entered = true;\
        } \
        while(sstm->peek() == ')') { \
            sstm->seekg((int)sstm->tellg() + 1); \
            entered = true; \
        } \
        while(sstm->peek() == '{') { \
            sstm->seekg((int)sstm->tellg() + 1); \
            entered = true; \
        } \
        while(sstm->peek() == '}') { \
            sstm->seekg((int)sstm->tellg() + 1); \
            entered = true; \
        } \
        if(!entered) { \
            break; \
        } \
    }*/

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


std::vector<XCubeSatToken*> *XCubeSatCompiler::tokenizer()
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

    discard_blanks(this->fileString);
    while(!this->fileString->eof() && !error) {
        c = fileString->get();
        switch(c) {
            case '(':
                if(insideStr) break;
                if(!buffer.empty()) {
                    t = new XCubeSatToken(&buffer);
                    this->tokens->push_back(t);
                    buffer.clear();
                }
                paren.push(c);
                discard_blanks(this->fileString);
                continue;
            case '{':
                if(insideStr) break;
                if(!buffer.empty()) {
                    t = new XCubeSatToken(&buffer);
                    this->tokens->push_back(t);
                    buffer.clear();
                }
                buffer.push_back(c);
                t = new XCubeSatToken(&buffer);
                this->tokens->push_back(t);
                buffer.clear();
                paren.push(c);
                discard_blanks(this->fileString);
                continue;
            case '}':
                if(insideStr) break;
                if(!buffer.empty()) {
                    t = new XCubeSatToken(&buffer);
                    this->tokens->push_back(t);
                    buffer.clear();
                }
                buffer.push_back(c);
                t = new XCubeSatToken(&buffer);
                this->tokens->push_back(t);
                buffer.clear();
                if(!paren.empty()) 
                    paren.pop();
                else 
                    error = true;
                discard_blanks(this->fileString);
                continue;
            case ')':
                if(insideStr) break;
                if(!buffer.empty()) {
                    t = new XCubeSatToken(&buffer);
                    this->tokens->push_back(t);
                    buffer.clear();
                }
                if(!paren.empty()) 
                    paren.pop();
                else 
                    error = true;
                discard_blanks(this->fileString);
                continue;
            case '=':
            case ',':
                if(insideStr) break;
                if(!buffer.empty()) {
                    t = new XCubeSatToken(&buffer);
                    this->tokens->push_back(t);
                    buffer.clear();
                }
                discard_blanks(this->fileString);
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
                discard_blanks(this->fileString);
                continue;
            case '#':
                fileString->getline(garbage, MAX_COMMENTS_SIZE);
                discard_blanks(this->fileString);
                continue;

        }
        c = toupper(c);
        buffer.push_back(c);

        if(!insideStr)
            discard_blanks(this->fileString);
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

bool XCubeSatCompiler::are_there_syntax_errors()
{
    if(this->error)
        return true;
    return false;
}

Glib::ustring XCubeSatCompiler::get_syntax_errors()
{
    return this->log;
}