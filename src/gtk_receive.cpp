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

#include <iostream>

#include "gtk_receive.hpp"
#include "tools.hpp"
#include "log.hpp"

/*  --------------------------------------------------------  */
/* Auxiliary functions definitions */
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
GtkReceive::GtkReceive(sigc::connection *mainWindowConn, std::queue<std::string> *inputBuffer, sigc::slot<bool> timeout_slot)
{
    this->builder = Gtk::Builder::create_from_file(MAIN_WINDOW_GLADE);

    this->builder->get_widget(RECEIVE_WINDOW, this->receiveWindow);

    this->builder->get_widget(SAT_NAME_WIDGET, this->satName);
    this->builder->get_widget(SAT_EL_WIDGET, this->satEl);
    this->builder->get_widget(SAT_AZ_WIDGET, this->satAz);
    this->builder->get_widget(STATUS_WIDGET, this->status);
    this->builder->get_widget(SPINNER_RECEIVE_WIDGET, this->spinner);

    this->satEl->set_text("1.0");
    this->satAz->set_text("1.0");

    this->mainWindowConn = mainWindowConn;
    this->timeout_slot = timeout_slot;
    this->inputBuffer = inputBuffer;

    /* Callbacks Connection */
    this->receiveWindow->signal_hide().connect(sigc::mem_fun(*this, &GtkReceive::close));

    this->fifo = new std::ifstream(FIFO_FILE);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void GtkReceive::close()
{
    this->conn.disconnect();

    this->satEl->set_text("1.0");
    this->satAz->set_text("1.0");
    this->status->set_text("Idle");
    this->spinner->stop();

    *this->mainWindowConn = Glib::signal_timeout().connect(this->timeout_slot, UPDATE_RATE);

    return;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void GtkReceive::start_receive_window()
{
    this->mainWindowConn->disconnect();

    this->update_curr_satellite();

    this->lastSatName = satName->get_text();

    sigc::slot<bool> timeout_slot = sigc::mem_fun(*this, &GtkReceive::update_window);
    this->conn = Glib::signal_timeout().connect(timeout_slot, UPDATE_RATE);

    this->status->set_text("Receiving...");
    this->spinner->start();

    this->receiveWindow->show();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool GtkReceive::wait_for_input()
{
    try {
        if (std::stof(this->satEl->get_text()) < 0.0 || !this->is_input_buffer_empty() || lastSatName.compare(satName->get_text()) != 0) {
            return true;
        }
    }
    catch (const std::exception &ex) {
        return false;
    }

    return false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool GtkReceive::update_window()
{
    this->update_curr_satellite();

    try {
        if (std::stof(this->satEl->get_text()) < 0.0f || !this->is_input_buffer_empty() || lastSatName.compare(satName->get_text()) != 0) {
            this->receiveWindow->hide();
        }
    }
    catch (const std::exception &ex) {
        /* What to do here ? */
    }

    return true;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void GtkReceive::update_curr_satellite()
{
    fifo_file_model *m = read_fifo_format(&this->fifo);

    /* Renders the new info in CurrSat frame */
    if(m->satName == NULL) {
        m->satName = new std::string("Not Found");
        /* If not found may be because the FIFO file 
         * is not good anymore, than we close it 
         * and reopen it */
        this->fifo->close();
        this->fifo = new std::ifstream(FIFO_FILE);
    }
    this->satName->set_text(*m->satName);

    if(m->el == NULL) m->el = new std::string("Not Set");
    this->satEl->set_text(*m->el);

    if(m->az == NULL) m->az = new std::string("Not Set");
    this->satAz->set_text(*m->az);
}
/*  --------------------------------------------------------  */  

/*  --------------------------------------------------------  */
bool GtkReceive::is_input_buffer_empty()
{
    if (this->inputBuffer->size() == 0) {
        return true;
    }

    return false;
}
/*  --------------------------------------------------------  */
