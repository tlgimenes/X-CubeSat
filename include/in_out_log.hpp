/*
 * CLASS IN_OUT_LOG : A log of the inout/output operation.
 *
 */

#ifndef IN_OUT_LOG_HPP
#define IN_OUT_LOG_HPP

#include <string>

typedef enum log_nature_t {
    SEND,
    RECEIVE,
} log_nature;

class InOutLog
{
    private:
        log_nature  nature;
        std::string *id;
        bool success;

        std::string *data;
    public:
        InOutLog(log_nature nature, std::string *id, std::string *data, bool success);
        InOutLog(log_nature nature, std::string *id, std::string *data);
        InOutLog(log_nature nature, std::string *id);
        InOutLog(log_nature nature, std::string *data, bool success);
        log_nature  GetNature();
        std::string *GetId();
        std::string *GetData();
        bool isSuccessful();
};

#endif
