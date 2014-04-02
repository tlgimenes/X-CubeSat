/* X-CubeSat Controler: Real-time communication with satellite program

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

#ifndef XCUBESAT_TOKEN_HPP
#define XCUBESAT_TOKEN_HPP

#include <gtkmm.h>

typedef enum token_t
{
    SEND,
    SAVE,
    IF,
    EQ,
    LL,
    LEQ,
    FILE2,
    APPENDDATE,
    FORMAT,
    RECEIVE,
    STRING,
    EQUALS,
    VARIABLE,
    DECLARE,
    SET,
    LPAREN,
    RPAREN
} token;

typedef union data_t {
    Glib::ustring *sdata;
    bool        bdata;
    int         idata;
} data_t;

class XCubeSatToken
{
    private:
        token type;
        data_t data;

    public:
        XCubeSatToken(token t);
        XCubeSatToken(Glib::ustring *str);

        void set_data(Glib::ustring *str);
        void set_data(bool b);
        void set_data(int i);

        token get_type();
        Glib::ustring* get_value_str();
        int get_value_int();
};

#endif
