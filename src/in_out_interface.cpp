/*
 * CLASS IN_OUT_INTERFACE : This class provides 
 * methods for operating input output for the program.
 *
 */

#include "in_out_interface.hpp"
#include "init.hpp"
#include "log.hpp"

InOutInterface::InOutInterface()
{
    this->port = new Port(NULL);
    this->deviceName = DEFAULT_OUTPUT;
}

InOutInterface::InOutInterface(Glib::ustring *deviceName)
{
    this->port = new Port(deviceName->c_str());
    this->deviceName = *deviceName;
}

bool InOutInterface::open(Glib::ustring deviceName, int speed)
{
    if(this->port != NULL) {
        return this->port->open_port(deviceName.c_str(), speed);
    }

    return false;
}

InOutLog * InOutInterface::write(Glib::ustring *data)
{
    InOutLog *log = NULL;

    if(this->port->is_oppenned()) {
        this->port->write_to_port((char*)data->c_str(), data->size());
        log = new InOutLog(SEND_LOG, NULL, true);
    }
    else {
        log = new InOutLog(SEND_LOG, new Glib::ustring("port not oppenned"), false);
        Log::LogWarn(LEVEL_LOG_WARNING, "Couldn't write because the port was not oppenned", __FILE__, __LINE__);
    }

    return log;
}

InOutLog * InOutInterface::read(char delim)
{
    Glib::ustring *data = new Glib::ustring();
    InOutLog *log = NULL;

    if(this->port->is_oppenned()) {
        *data = this->port->read_port(delim);
        log = new InOutLog(RECEIVE_LOG, data, true);
    }
    else {
        log = new InOutLog(SEND_LOG, new Glib::ustring("port not oppenned"), false);
        Log::LogWarn(LEVEL_LOG_WARNING, "Couldn't read because the port was not oppenned", __FILE__, __LINE__);
    }

    return log;
}

InOutLog * InOutInterface::read(size_t count)
{
    char *data = new char[count];
    InOutLog *log = NULL;

    if(this->port->is_oppenned()) {
        this->port->read_port(data, count);
        log = new InOutLog(RECEIVE_LOG, new Glib::ustring(data), true);
    }
    else {
        log = new InOutLog(SEND_LOG, new Glib::ustring("port not oppenned"), false);
        Log::LogWarn(LEVEL_LOG_WARNING, "Couldn't read because the port was not oppenned", __FILE__, __LINE__);
    }

    return log;
}

bool InOutInterface::set_device_speed(int speed)
{
    if(speed >= 0) {
        return this->port->set_speed(speed);
    }

    return false;
}

Glib::ustring InOutInterface::get_device_name()
{
    return this->deviceName;
}

bool InOutInterface::is_configured()
{
    if(this->port != NULL)
        return this->port->is_configured();

    return false;
}

bool InOutInterface::is_oppenned()
{
    if(this->port != NULL)
        return this->port->is_oppenned();

    return false;
}
