/*
 * CLASS SATELLITE : Informations about an specific satellite.
 *
 */

#ifndef SATELLITE_HPP
#define SATELLITE_HPP

#include <gtkmm.h>
#include <string>

class Satellite
{
    private:
        Glib::ustring *nickname;
        int         *id;

    public:
        Satellite(Glib::ustring *nickname, int id);
        Satellite(Glib::ustring *nickname);

        Glib::ustring *get_nickname();
};

#endif
