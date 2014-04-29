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

#include <sstream>
#include <ostream>
#include <iostream>
#include <gtkmm/texttag.h>

#include "terminal.hpp"
#include "log.hpp"
#include "defs.hpp"
#include "modems.hpp"

/*  --------------------------------------------------------  */
/* Constructor */
/*  --------------------------------------------------------  */
Terminal::Terminal(InOutInterface *interface, Glib::RefPtr<Gtk::TextBuffer> buffer)
{
    this->buffer = buffer;
    this->interface = interface;

    /*  Set the mode of work for this terminal and 
     *  the max number of lines for this terminal */
    this->mode = MODEM_CONFIG;
    this->max_num_lines = MAX_BUFFER_SIZE;

    /*  Set Tags */
    this->notEditableTag = this->buffer->create_tag(NON_EDITABLE_TAG_NAME);
    this->notEditableTag->property_editable().set_value(false);

    /* Connect callbacks for edited Gtk::TextBuffer by user */
    this->buffer->signal_end_user_action().connect(sigc::mem_fun(*this, &Terminal::on_end_user_action));
    this->buffer->signal_begin_user_action().connect(sigc::mem_fun(*this, &Terminal::on_begin_user_action));
    this->buffer->signal_insert().connect(sigc::mem_fun(*this, &Terminal::on_my_insert));
    this->erase = true;

    /* Sets the callback for to read from the serial port */
    if(this->interface != NULL && this->interface->is_open())
        this->interface->set_read_callback(sigc::mem_fun(*this, &Terminal::update_read));
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor */
/*  --------------------------------------------------------  */
Terminal::Terminal(InOutInterface *interface)
{
    this->interface = interface;

    /*  Set the mode of work for this terminal and 
     *  the max number of lines for this terminal */
    this->mode = MODEM_CONFIG;
    this->max_num_lines = MAX_BUFFER_SIZE;

    /* Sets the callback for to read from the serial port */
    if(this->interface != NULL && this->interface->is_open())
        this->interface->set_read_callback(sigc::mem_fun(*this, &Terminal::update_read));
}
/*  --------------------------------------------------------  */

/* Clears a buffer */
#define CLEAR_BUFFER(buffer) \
    if(this->mode == MODEM_CONFIG) { \
        std::queue<std::string> empty; \
        std::swap(this->output, empty); \
    }
/*  --------------------------------------------------------  */
/*  Reads what arrives from the modem/device that it is 
 *  connected in and shows in the terminal module */
void Terminal::update()
{
    /*  DO NOT CHANGE THER ORDER */
    this->update_write ();
    this->update_buffer();

    CLEAR_BUFFER(this->output);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/*  Callback for receiving data from modem */
void Terminal::update_read(Glib::ustring data)
{
    Glib::ustring str;
    char c;
    
    for (unsigned int i = 0; i < data.size(); ++i) {
        c = data.at(i);
        if (c < 32 || c >= 0x7f) str.push_back(c); /* Remove non ASCII char */
        if (c == '\n') {
            input.push(str);
            str.clear();
        }
    }

    return;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Write to the interface and decrease the output buffer */
void Terminal::update_write()
{
    if(this->interface != NULL && this->interface->is_open()) {
        for (unsigned int i = 0; i < this->output.size(); ++i) {
            Glib::ustring *str = new Glib::ustring(this->output.front());
            this->interface->write(str);
            this->output.push(this->output.front());
            this->output.pop();
        }
    }
}
/*  --------------------------------------------------------  */

#define WRITE_TO_TEXTVIEW(buffer) \
    for (unsigned int i = 0; i < buffer.size(); i++) { \
        str = buffer.front(); \
        buffer.pop(); \
        buff << str << "\n"; \
        if(this->mode == MODEM_FREE) \
        buffer.push(str); \
    }

#define SMART_ERASE_BUFFER(buffer) \
    for (unsigned int i = 0; i < buffer.size(); i++) { \
        str = buffer.front(); \
        buffer.pop(); \
        if(this->mode == MODEM_FREE) \
        buffer.push(str); \
    }

/*  --------------------------------------------------------  */
/* Puts the input buffer in the Gtk::TextBuffer */
void Terminal::update_buffer()
{
    if(this->input.size() > 0 || this->output.size() > 0) {
        unsigned int lines = this->buffer->get_line_count();
        std::string str = this->buffer->get_text();
        std::string aux;
        std::stringstream buff;
        unsigned int size;
        buff << str;

        /*  Open the necessary space for the buffer */
        int count = input.size() + this->output.size() + lines - max_num_lines;
        for (int i = 0; i < count; ++i) {
            std::getline(buff, aux);
            size = aux.size();
            str.erase(0, size+1);
        }

        /*  Resets the new buffer */
        buff.str(std::string());
        buff << str; /*  Assignes the new string to the buffer */

        /* Writes to buffer */
        WRITE_TO_TEXTVIEW(this->input);
        if(this->mode == MODEM_CONFIG) {
            SMART_ERASE_BUFFER(this->output);
        }
        else {
            WRITE_TO_TEXTVIEW (this->output);
        }

       this->buffer->set_text(buff.str());
       this->buffer->apply_tag(this->notEditableTag, buffer->begin(), buffer->end());
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool Terminal::write_to_device(std::string str)
{
    if(mode == MODEM_FREE) {
        this->output.push(str);
        return true;
    }

    return false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool Terminal::read_from_device(std::string *str) 
{
    if(mode == MODEM_FREE) {
        *str = this->input.front();
        this->input.pop();
        return true;
    }

    return false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void Terminal::change_mode()
{
    if(this->mode == MODEM_CONFIG) {
        this->mode = MODEM_FREE;
        Log::LogWarn(LEVEL_LOG_INFO, "Terminal mode changed to MODEM FREE sucessfully", __FILE__, __LINE__);
    }
    else {
        this->mode = MODEM_CONFIG;
        Log::LogWarn(LEVEL_LOG_INFO, "Terminal mode changed to MODEM CONFIG sucessfully", __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void Terminal::on_end_user_action()
{
    std::string buff = this->buffer->get_text();

    if(!this->erase) {
        if(buff.back() == '\n') {
            this->output.push(this->inputUserBuffer);
            this->inputUserBuffer.clear();
        }
        else {
            this->inputUserBuffer.push_back(buff.back());
        }
    }
    else if(!this->inputUserBuffer.empty()) {
        this->inputUserBuffer.pop_back();
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void Terminal::on_begin_user_action()
{
    this->erase = true;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void Terminal::on_my_insert(const Gtk::TextBuffer::iterator& pos, const Glib::ustring &text, int bytes)
{
    this->erase = false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void Terminal::set_buffer(Glib::RefPtr<Gtk::TextBuffer> buffer)
{
    this->buffer = buffer;

    /*  Set Tags */
    this->notEditableTag = this->buffer->create_tag(NON_EDITABLE_TAG_NAME);
    this->notEditableTag->property_editable().set_value(false);

    /* Connect callbacks for edited Gtk::TextBuffer by user */
    this->buffer->signal_end_user_action().connect(sigc::mem_fun(*this, &Terminal::on_end_user_action));
    this->buffer->signal_begin_user_action().connect(sigc::mem_fun(*this, &Terminal::on_begin_user_action));
    this->buffer->signal_insert().connect(sigc::mem_fun(*this, &Terminal::on_my_insert));
    this->erase = true;
}
/*  --------------------------------------------------------  */

bool Terminal::set_interface(Glib::ustring deviceName, int speed)
{
    try {
        if(this->interface != NULL)
            return false;
        else
            this->interface = new InOutInterface(&deviceName, speed);

        /* Sets the callback for to read from the serial port */
        if(this->interface != NULL && this->interface->is_open())
            this->interface->set_read_callback(sigc::mem_fun(*this, &Terminal::update_read));

        return this->interface->is_open();
    } 
    catch(std::exception &ex) {
        Glib::ustring err = "Unable to open device because: \"";
        err.append(ex.what()); err.append("\"");
        Log::LogWarn(LEVEL_LOG_WARNING, err.c_str(), __FILE__, __LINE__);
    }

    return false;
}

/*  --------------------------------------------------------  */
InOutInterface *Terminal::get_interface()
{
    return this->interface;
}
/*  --------------------------------------------------------  */
