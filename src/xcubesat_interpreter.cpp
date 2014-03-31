/*
 * CLASS XCUBESAT_INTERPRETER : This class defines 
 * methods for interpreting a command file.
 *
 */

#include <iostream>
#include <vector>
#include <typeinfo>

#include "xcubesat_interpreter.hpp"
#include "function.hpp"

XCubeSatInterpreter::XCubeSatInterpreter(InOutInterface *inOutInterface) : Interpreter(inOutInterface)
{
    this->compiler = new XCubeSatCompiler();
}

XCubeSatInterpreter::XCubeSatInterpreter(Glib::ustring *portName) : Interpreter(portName)
{
    this->compiler = new XCubeSatCompiler();
}

InOutLog *XCubeSatInterpreter::interpret(Glib::ustring *text, std::unordered_map<std::string, std::string> *alias, Glib::ustring *satName)
{
    try {
        std::stringstream *sstr = new std::stringstream(text->c_str());

        this->runQueue = *this->compiler->compile(alias, sstr, this->inOutInterface);

        Function *f;

        while(!this->runQueue.empty()) {
            f = this->runQueue.front();
            this->runQueue.erase(this->runQueue.begin());
            f->run(&this->runQueue, satName);
            std::cout << std::endl;
        }
        /*
         * TODO: FINISH HERE
         */
    }
    catch(std::bad_typeid *e) {
        Log::LogWarn(LEVEL_LOG_INFO, "Error while compilling file", __FILE__, __LINE__);
    }
    catch(std::invalid_argument a) {
        Log::LogWarn(LEVEL_LOG_INFO, "Error while compilling file", __FILE__, __LINE__);
    }

    return new InOutLog(SEND_LOG, NULL, true);
}


bool *XCubeSatInterpreter::are_there_syntax_errors(Glib::ustring *text, std::unordered_map<std::string, std::string> *alias) 
{
    std::stringstream *sstr = new std::stringstream(text->c_str());
    bool *res;

    try {
        this->compiler->compile(alias, sstr, this->inOutInterface);

        res = new bool(this->compiler->are_there_syntax_errors());
    }
    catch(std::bad_typeid *e) {
        Log::LogWarn(LEVEL_LOG_INFO, "Error while compilling file", __FILE__, __LINE__);
        res = new bool(true);
    }

    return res;
}
