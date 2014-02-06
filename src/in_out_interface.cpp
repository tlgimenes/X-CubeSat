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
    this->portName = NULL;
    this->portNameStatus = NULL;
    this->portSpeedComboBox = NULL;
    this->portSpeedStatus = NULL;
}

InOutInterface::InOutInterface(Glib::ustring *deviceName)
{
    this->port = new Port(deviceName->c_str());
    this->deviceName = *deviceName;
    this->portName = NULL;
    this->portNameStatus = NULL;
    this->portSpeedComboBox = NULL;
    this->portSpeedStatus = NULL;
}

InOutLog * InOutInterface::Write(Glib::ustring *data)
{
    InOutLog *log = NULL;

    if(this->port->IsOppenned()) {
        this->port->Write((char*)data->c_str(), data->size());
        log = new InOutLog(SEND, NULL, true);
    }
    else {
        log = new InOutLog(SEND, new Glib::ustring("port not oppenned"), false);
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
        log = new InOutLog(RECEIVE, new Glib::ustring(data), true);
    }
    else {
        log = new InOutLog(SEND, new Glib::ustring("port not oppenned"), false);
        Log::LogWarn(LEVEL_LOG_WARNING, "Couldn't read because the port was not oppenned", __FILE__, __LINE__);
    }

    return log;
}

void InOutInterface::SetDeviceName()
{
    try {
        Glib::ustring deviceName = this->portName->get_text();
        Gtk::TreeModel::Row row = (*this->portSpeedComboBox->get_active());

        Glib::ustring speedName = row.get_value(this->model.speedName);
        Glib::ustring iconName;

        int speed = std::stoi(speedName.c_str());

        if(this->port->Open(deviceName.c_str(), speed)) 
            iconName = "gtk-yes";
        else 
            iconName = "gtk-no";
        this->portNameStatus->clear();
        this->portNameStatus->set_from_icon_name(iconName, Gtk::ICON_SIZE_BUTTON);

        if(this->port->IsConfigured()) 
            iconName = "gtk-yes";
        else 
            iconName = "gtk-no";
        this->portSpeedStatus->clear();
        this->portSpeedStatus->set_from_icon_name(iconName, Gtk::ICON_SIZE_BUTTON);
    } 
    catch(std::invalid_argument &exp) {
        Log::LogWarn(LEVEL_LOG_INFO, "Choose a speed", __FILE__, __LINE__);
        return;
    }
}

void InOutInterface::SetDeviceSpeed()
{
    try {
        Gtk::TreeModel::Row row = (*this->portSpeedComboBox->get_active());

        Glib::ustring speedName = row.get_value(model.speedName);
        Glib::ustring iconName;

        int speed = std::stoi(speedName.c_str());

        if(!this->port->SetSpeed(speed)) {
            iconName = "gtk-no";
        }
        else {
            iconName = "gtk-yes";
        }
        this->portSpeedStatus->clear();
        this->portSpeedStatus->set_from_icon_name(iconName, Gtk::ICON_SIZE_BUTTON);
    } 
    catch (std::invalid_argument& arg) {
        Log::LogWarn(LEVEL_LOG_INFO, "Choose a speed", __FILE__, __LINE__);
        return;
    }
}

void InOutInterface::InitConfigFrameGtk(Gtk::Entry *portName, Gtk::Image *portNameStatus, Gtk::ComboBox *portSpeedComboBox, Gtk::Image *portSpeedStatus)
{
    this->portName = portName;
    this->portNameStatus = portNameStatus;
    this->portSpeedComboBox = portSpeedComboBox;
    this->portSpeedStatus = portSpeedStatus;

    if(this->port->IsOppenned()) {
        this->portName->set_text(this->deviceName);
        this->portNameStatus->set_from_icon_name("gtk-yes", Gtk::ICON_SIZE_BUTTON);
    }
    if(this->port->IsConfigured()) {
        this->portSpeedStatus->set_from_icon_name("gtk-yes", Gtk::ICON_SIZE_BUTTON);
    }

    this->portName->signal_activate().connect(sigc::mem_fun(*this, &InOutInterface::SetDeviceName));

    this->portSpeedComboBox->signal_changed().connect(sigc::mem_fun(*this, &InOutInterface::SetDeviceSpeed));
}
