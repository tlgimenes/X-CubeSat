/*
 * CLASS XCUBESAT_INTERPRETER : This class defines 
 * methods for interpreting a command file.
 *
 */
/* X-CubeSat Controller: Real-time communication with satellite program

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

#include <iostream>
#include <vector>
#include <typeinfo>

#include "xcubesat_interpreter.hpp"
#include "function.hpp"

/*  --------------------------------------------------------  */
/*
 *  Constructors
 */
XCubeSatInterpreter::XCubeSatInterpreter(Terminal *term) : Interpreter(term)
{
    this->compiler = new XCubeSatCompiler();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/*
 *  Inteprets the script and creates errors dialogs in case of errors. 
 */
InOutLog *XCubeSatInterpreter::interpret(Glib::ustring *text, std::unordered_map<std::string, std::string> *alias, Glib::ustring *satName)
{
    try {
        std::stringstream *sstr = new std::stringstream(text->c_str());

        this->runQueue = *this->compiler->compile(alias, sstr, this->term);

        Function *f;

        while(!this->runQueue.empty()) {
            f = this->runQueue.front();
            this->runQueue.erase(this->runQueue.begin());
            f->run(&this->runQueue, satName);
            //std::cout << std::endl;
        }
    }
    catch(std::bad_typeid *e) {
        Log::LogWarn(LEVEL_LOG_INFO, "Error while compilling file", __FILE__, __LINE__);
    }
    catch(std::invalid_argument a) {
        Log::LogWarn(LEVEL_LOG_INFO, "Error while compilling file", __FILE__, __LINE__);
    }

    return new InOutLog(SEND_LOG, NULL, true);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/*
 *  Check if there are syntax errors
 */
bool *XCubeSatInterpreter::are_there_syntax_errors(Glib::ustring *text, std::unordered_map<std::string, std::string> *alias) 
{
    std::stringstream *sstr = new std::stringstream(text->c_str());
    bool *res;

    try {
        this->compiler->compile(alias, sstr, this->term);

        res = new bool(this->compiler->are_there_syntax_errors());
    }
    catch(std::bad_typeid *e) {
        Log::LogWarn(LEVEL_LOG_INFO, "Error while compilling file", __FILE__, __LINE__);
        res = new bool(true);
    }

    return res;
}
/*  --------------------------------------------------------  */
