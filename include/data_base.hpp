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
        static std::stringstream *get_section(Glib::ustring key);
        static std::stringstream *get_scripts(Glib::ustring key);
        static std::stringstream *get_alias  (Glib::ustring key);
        static std::stringstream *get_sats   (Glib::ustring key);

        static bool exists_section(Glib::ustring key);
        static bool exists_script (Glib::ustring key);
        static bool exists_alias  (Glib::ustring key);
        static bool exists_sats   (Glib::ustring key);
};

#endif
