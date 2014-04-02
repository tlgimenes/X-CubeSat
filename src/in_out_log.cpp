/*
 * CLASS IN_OUT_LOG : A log of the inout/output operation.
 *
 */

#include <ctime>

#include "in_out_log.hpp"
#include "log.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
InOutLog::InOutLog(log_nature nature, Glib::ustring *id, Glib::ustring *data, bool success)
{
    this->nature = nature;
    this->id = id;
    this->data = data;
    this->success = success;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor
 */
InOutLog::InOutLog(log_nature nature, Glib::ustring *id, Glib::ustring *data)
{
    this->nature = nature;
    this->id = id;
    this->data = data;
    this->success = true;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor
 */
InOutLog::InOutLog(log_nature nature, Glib::ustring *id)
{
    this->nature = nature;
    this->id = id;
    this->data = NULL;
    this->success = true;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor
 */
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
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the nature of the log (SEND_LOG or RECEIVE_LOG)
 */
log_nature InOutLog::get_nature()
{
    return this->nature;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the id of the log
 */
Glib::ustring * InOutLog::get_id()
{
    return this->id;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns the data of this log
 */
Glib::ustring * InOutLog::get_data()
{
    return this->data;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Returns true if it was sucessful, false otherwise
 */
bool InOutLog::is_successful()
{
    return this->success;
}
/*  --------------------------------------------------------  */
