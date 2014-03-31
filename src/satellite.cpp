/*
 * CLASS SATELLITE : Informations about an specific satellite.
 *
 */

#include "satellite.hpp"

Satellite::Satellite(Glib::ustring *nickname)
{
    this->nickname = nickname;
}

Glib::ustring *Satellite::get_nickname()
{
    return this->nickname;
}

