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

#ifndef GTK_RECEIVE_HPP
#define GTK_RECEIVE_HPP

#include <gtkmm.h>
#include <gtkmm/builder.h>
#include <fstream>
#include <queue>

#include "defs.hpp"

/* Window name */
#define RECEIVE_WINDOW "statusReceiving"

/* Widgets definitions */
#define SPINNER_RECEIVE_WIDGET  "spinner1"
#define SAT_NAME_WIDGET "satNam"
#define SAT_EL_WIDGET   "satEl"
#define SAT_AZ_WIDGET   "satAz"
#define STATUS_WIDGET   "statusLabel"

class GtkReceive 
{
    private:
        Gtk::Main *main;

        Glib::RefPtr<Gtk::Builder> builder;
        Gtk::ApplicationWindow *receiveWindow;

        Gtk::Label *satName;
        Gtk::Label *satEl, *satAz;
        Gtk::Label *status;
        Gtk::Spinner *spinner;

        std::queue<std::string> *inputBuffer;

        sigc::connection *mainWindowConn;
        sigc::slot<bool> timeout_slot;

        sigc::connection conn;

        std::ifstream *fifo;

        Glib::ustring lastSatName;
        Glib::ustring currSatName;

    protected:
        void close();

        bool update_window();

        void update_curr_satellite();

        bool is_input_buffer_empty();

    public:
        GtkReceive(sigc::connection *mainWindowConn, std::queue<std::string> *inputBuffer, sigc::slot<bool> timeout_slot, Gtk::Main *main);

        /* This function waits for the terminal to have
         * received something (a line) or the satellite 
         * be below the horizon. */
        void wait_for_input();
};

#endif /* GTK_RECEIVE_HPP */
