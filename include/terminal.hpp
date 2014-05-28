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

#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <gtkmm.h>
#include <sigc++/sigc++.h>
#include <queue>

#include "in_out_interface.hpp"
#include "modems.hpp"
#include "gtk_receive.hpp"

typedef enum working_mode_t
{
    MODEM_MANUAL_MODE,
    MODEM_AUTO_MODE
} working_mode;

class Terminal : public sigc::trackable
{
    private:
        unsigned int max_num_lines;
        InOutInterface *interface;
        std::queue<std::string> output;
        std::queue<std::string> input;
        Glib::ustring inputPortBuffer; /* Buffer containing data came from the port */
        std::string inputUserBuffer; /* Buffer containing data came from the user */
        working_mode mode;
        bool erase;
        Modem *modem;

        bool read_locker; // Locks the read_from_device function

        Gtk::TextView *textView;
        Glib::RefPtr<Gtk::TextBuffer> buffer;
        Glib::RefPtr<Gtk::TextTag> notEditableTag;

        GtkReceive *rec;

    protected:
        void update_read(Glib::ustring data);
        void update_write();
        void update_buffer();

        void on_begin_user_action();
        void on_end_user_action();
        void on_my_insert(const Gtk::TextBuffer::iterator& pos, const Glib::ustring &text, int bytes);

        void modem_name_changed_combobox_cb();
        void modem_mode_changed_combobox_cb();

        bool block_read();
    public:
        Terminal(InOutInterface *interface, Glib::RefPtr<Gtk::TextBuffer> buffer, Gtk::TextView *textView);
        Terminal(InOutInterface *interface);

        void update();

        bool write_to_device (std::string  str);
        bool read_from_device(std::string *str);

        void change_config_mode();

        /*  Gets and Sets */
        bool set_interface(Glib::ustring deviceName, int speed);
        void set_buffer(Glib::RefPtr<Gtk::TextBuffer> buffer);
        void set_textview(Gtk::TextView *textView);
        void set_modem(Modem *modem);
        void set_gtk_receive(GtkReceive *rec);
        InOutInterface *get_interface();
        std::queue<std::string> *get_input_buffer();

        bool is_read_locked(); // If terminal is locked for reading
        void unlock_read();

        void close_gtk_receive_window();
};

#endif
