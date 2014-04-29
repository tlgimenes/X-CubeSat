/* X-CubeSat Controler: Real-time communication with satellite program

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

typedef enum working_mode_t
{
    MODEM_CONFIG,
    MODEM_FREE
} working_mode;

class Terminal : public sigc::trackable
{
    private:
        unsigned int max_num_lines;
        InOutInterface *interface;
        std::queue<std::string> output;
        std::queue<std::string> input;
        std::string inputUserBuffer;
        working_mode mode;
        bool erase;

        Glib::RefPtr<Gtk::TextBuffer> buffer;
        Glib::RefPtr<Gtk::TextTag> notEditableTag;

    protected:
        void update_read(Glib::ustring data);
        void update_write();
        void update_buffer();

        void on_begin_user_action();
        void on_end_user_action();
        void on_my_insert(const Gtk::TextBuffer::iterator& pos, const Glib::ustring &text, int bytes);

    public:
        Terminal(InOutInterface *interface, Glib::RefPtr<Gtk::TextBuffer> buffer);
        Terminal(InOutInterface *interface);

        void update();

        bool write_to_device (std::string  str);
        bool read_from_device(std::string *str);

        void change_mode();

        /*  Gets and Sets */
        bool set_interface(Glib::ustring deviceName, int speed);
        void set_buffer(Glib::RefPtr<Gtk::TextBuffer> buffer);
        InOutInterface *get_interface();
};

#endif
