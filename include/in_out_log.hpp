/*
 * CLASS IN_OUT_LOG : A log of the inout/output operation.
 *
 */

#ifndef IN_OUT_LOG_HPP
#define IN_OUT_LOG_HPP

#include <gtkmm.h>

typedef enum log_nature_t {
    SEND,
    RECEIVE,
} log_nature;

class InOutLog
{
    private:
        log_nature  nature;
        Glib::ustring *id;
        bool success;

        Glib::ustring *data;
    public:
        InOutLog(log_nature nature, Glib::ustring *id, Glib::ustring *data, bool success);
        InOutLog(log_nature nature, Glib::ustring *id, Glib::ustring *data);
        InOutLog(log_nature nature, Glib::ustring *id);
        InOutLog(log_nature nature, Glib::ustring *data, bool success);
        log_nature  GetNature();
        Glib::ustring *GetId();
        Glib::ustring *GetData();
        bool isSuccessful();
};

#endif
