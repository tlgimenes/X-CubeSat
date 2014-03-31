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
