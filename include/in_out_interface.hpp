/*
 * CLASS IN_OUT_INTERFACE : This class provides 
 * methods for operating input output for the program.
 *
 */

#ifndef IN_OUT_INTERFACE_HPP
#define IN_OUT_INTERFACE_HPP

#include <gtkmm.h>
#include "port.hpp"
#include "in_out_log.hpp"
#include "models.hpp"

class InOutInterface
{
    private:
        Port *port;
        Glib::ustring deviceName;
        
    public:
        InOutInterface();
        InOutInterface(Glib::ustring *portName);

        bool open(Glib::ustring deviceName, int speed);
        InOutLog * write(Glib::ustring *data);
        InOutLog * read(size_t count);
        InOutLog * read(char delim);

        bool set_device_speed(int);

        bool is_oppenned();
        bool is_configured();

        Glib::ustring get_device_name();

 
 //       void SetDeviceName();
 //       void SetDeviceSpeed();

 //       void InitConfigFrameGtk(Gtk::Entry *portName, Gtk::Image *portNameStatus, Gtk::ComboBox *portSpeedComboBox, Gtk::Image *portSpeedStatus);
};

#endif
