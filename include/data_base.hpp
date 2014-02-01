/*
 * CLASS DATA_BASE : this class is an interface between 
 * the program and the data base
 */

#ifndef DATA_BASE_HPP
#define DATA_BASE_HPP

#include <sstream>
#include <string>

class DataBase
{
    public:
        static std::stringstream *GetSection(std::string key);
        static std::stringstream *GetScripts(std::string key);
        static std::stringstream *GetAlias  (std::string key);
        static std::stringstream *GetSats   (std::string key);

        static bool existsSection(std::string key);
        static bool existsScript (std::string key);
        static bool existsAlias  (std::string key);
        static bool existsSats   (std::string key);
};

#endif
