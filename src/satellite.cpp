/*
 * CLASS SATELLITE : Informations about an specific satellite.
 *
 */

#include "satellite.hpp"

Satellite::Satellite(std::string *nickname)
{
    this->nickname = nickname;
}

std::string *Satellite::GetNickname()
{
    return this->nickname;
}
