/*
 * CLASS DATA_BASE : this class is an interface between 
 * the program and the data base
 */

#ifndef DATA_BASE_HPP
#define DATA_BASE_HPP

#include <gtkmm.h>
#include <sstream>

class DataBase
{
    public:
        static std::stringstream *GetSection(Glib::ustring key);
        static std::stringstream *GetScripts(Glib::ustring key);
        static std::stringstream *GetAlias  (Glib::ustring key);
        static std::stringstream *GetSats   (Glib::ustring key);

        static bool existsSection(Glib::ustring key);
        static bool existsScript (Glib::ustring key);
        static bool existsAlias  (Glib::ustring key);
        static bool existsSats   (Glib::ustring key);
};

#endif
