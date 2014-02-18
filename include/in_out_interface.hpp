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
        
        Gtk::Entry *portName;
        Gtk::Image *portNameStatus;
        Gtk::ComboBox *portSpeedComboBox;
        Gtk::Image *portSpeedStatus;
        ModelPortSpeedComboBox model;

    public:
        InOutInterface();
        InOutInterface(Glib::ustring *portName);
        InOutLog * Write(Glib::ustring *data);
        InOutLog * Read(size_t count);
 
        void SetDeviceName();
        void SetDeviceSpeed();

        void InitConfigFrameGtk(Gtk::Entry *portName, Gtk::Image *portNameStatus, Gtk::ComboBox *portSpeedComboBox, Gtk::Image *portSpeedStatus);
};

#endif
