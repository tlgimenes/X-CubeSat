/*
 * CLASS SATELLITE : Informations about an specific satellite.
 *
 */

#include "satellite.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
Satellite::Satellite(Glib::ustring *nickname)
{
    this->nickname = nickname;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Get the nickname of this satellite
 */
Glib::ustring *Satellite::get_nickname()
{
    return this->nickname;
}
/*  --------------------------------------------------------  */
