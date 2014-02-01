/*
 * CLASS SATELLITE : Informations about an specific satellite.
 *
 */

#ifndef SATELLITE_HPP
#define SATELLITE_HPP

#include <string>

class Satellite
{
    private:
        std::string *nickname;
        int         *id;

    public:
        Satellite(std::string *nickname, int id);
        Satellite(std::string *nickname);

        std::string *GetNickname();
};

#endif
