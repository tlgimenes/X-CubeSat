/* X-CubeSat Controller: Real-time communication with satellite program

 Copyright (C)  2014 - Tiago Lobato Gimenes

 Authors: Tiago Lobato Gimenes <tlgimenes@gmail.com>

 Comments, questions and bugreports should be submitted via
 https://github.com/tlgimenes/X-CubeSat
 More details can be found at the project home page:

 https://github.com/tlgimenes/X-CubeSat

 This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

 This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
along with this program; if not, visit http://www.fsf.org/
*/

#ifndef MODEMS_HPP
#define MODEMS_HPP

#include <vector>
#include <string>
#include <gtkmm.h>

#include "models.hpp"
#include "modem_default.hpp"
#include "modem_kantronics.hpp"
#include "modem_kenwood.hpp"

typedef enum modemModel_t {
    DEFAULT,
    KANTRONICS,
    KENWOOD
} modemModel_t;

class Modem
{
    private:
        std::vector<ModemDefault*> modems =
             { new ModemDefault(CONFIG), 
               new ModemKantronics(CONFIG), 
               new ModemKenwood(CONFIG)};

        modemModel_t currentModem;

        Glib::RefPtr<Gtk::ListStore> modemModeListStore;
        Gtk::ComboBoxText  *modemModeCombobox;
        Gtk::ComboBoxText  *modemNameCombobox;

        ModemModeComboBoxModel modemModeComboBoxModel;
        ModemNameComboBoxModel modemNameComboBoxModel;

    protected:
        void populate_name_combobox();
        void populate_mode_combobox();

  //      void modem_name_changed_combobox_cb();
  //      void modem_mode_changed_combobox_cb();

    public:
        Modem(modemModel_t model, Gtk::ComboBoxText *name, Gtk::ComboBoxText *mode);
        std::string REPLY_OEM();
        std::string OEM();

        Glib::ustring format_send(Glib::ustring);

        /* Returns a string containing the chars to 
         * change the modem mode */
        Glib::ustring update_modem_mode();      
        void          update_modem_name();

        /* Sets the chage mode callback to all of the
         * modems implementations */
        void set_change_modem_mode_cb(sigc::slot<void> change_mode_cb);
        void set_change_modem_name_cb(sigc::slot<void> change_name_cb);
};

#endif /* MODEMS_HPP */
