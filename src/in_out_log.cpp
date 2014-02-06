/*
 * CLASS IN_OUT_LOG : A log of the inout/output operation.
 *
 */

#include <ctime>

#include "in_out_log.hpp"
#include "log.hpp"

InOutLog::InOutLog(log_nature nature, Glib::ustring *id, Glib::ustring *data, bool success)
{
    this->nature = nature;
    this->id = id;
    this->data = data;
    this->success = success;
}

InOutLog::InOutLog(log_nature nature, Glib::ustring *id, Glib::ustring *data)
{
    this->nature = nature;
    this->id = id;
    this->data = data;
    this->success = true;
}

InOutLog::InOutLog(log_nature nature, Glib::ustring *id)
{
    this->nature = nature;
    this->id = id;
    this->data = NULL;
    this->success = true;
}

InOutLog::InOutLog(log_nature nature, Glib::ustring *data, bool success)
{
    time_t raw_time;

    // Sets the id as UTC time
    time(&raw_time);
    this->id = new Glib::ustring(asctime(gmtime(&raw_time)));

    this->nature = nature;
    this->data = data;
    this->success = success;
}

log_nature InOutLog::GetNature()
{
    return this->nature;
}

Glib::ustring * InOutLog::GetId()
{
    return this->id;
}

Glib::ustring * InOutLog::GetData()
{
    return this->data;
}

bool InOutLog::isSuccessful()
{
    return this->success;
}
