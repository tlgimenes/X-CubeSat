/*
 * CLASS IN_OUT_INTERFACE : This class provides 
 * methods for operating input output for the program.
 *
 */

#include "in_out_interface.hpp"
#include "log.hpp"

InOutInterface::InOutInterface()
{
    this->port = new Port(NULL);
}

InOutInterface::InOutInterface(std::string *portName)
{
    this->port = new Port(portName->c_str());
}

InOutLog * InOutInterface::Write(std::string *data)
{
    InOutLog *log = NULL;

    if(this->port->IsOppenned()) {
        this->port->Write((char*)data->c_str(), data->size());
        log = new InOutLog(SEND, NULL, true);
    }
    else {
        log = new InOutLog(SEND, new std::string("port not oppenned"), false);
        Log::LogWarn(LEVEL_LOG_WARNING, "Couldn't write because the port was not oppenned", __FILE__, __LINE__);
    }

    return log;
}

InOutLog * InOutInterface::Read(size_t count)
{
    char *data = new char[count];
    InOutLog *log = NULL;

    if(this->port->IsOppenned()) {
        this->port->Read(data, count);
        log = new InOutLog(RECEIVE, new std::string(data), true);
    }
    else {
        log = new InOutLog(SEND, new std::string("port not oppenned"), false);
        Log::LogWarn(LEVEL_LOG_WARNING, "Couldn't read because the port was not oppenned", __FILE__, __LINE__);
    }

    return log;
}
