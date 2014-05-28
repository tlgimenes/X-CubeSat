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
Terminal::Terminal(InOutInterface *interface, Glib::RefPtr<Gtk::TextBuffer> buffer, Gtk::TextView *textView)
{
    this->textView = textView;
    this->buffer = buffer;
    this->interface = interface;
    this->modem = NULL;
    this->rec = NULL;
    this->read_locker = false;

    /*  Set the mode of work for this terminal and 
     *  the max number of lines for this terminal */
    this->mode = MODEM_MANUAL_MODE;
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
    this->modem = NULL;
    this->rec = NULL;
    this->read_locker = false;

    /*  Set the mode of work for this terminal and 
     *  the max number of lines for this terminal */
    this->mode = MODEM_MANUAL_MODE;
    this->max_num_lines = MAX_BUFFER_SIZE;

    /* Sets the callback for to read from the serial port */
    if(this->interface != NULL && this->interface->is_open())
        this->interface->set_read_callback(sigc::mem_fun(*this, &Terminal::update_read));
}
/*  --------------------------------------------------------  */

/* Clears a buffer */
#define CLEAR_BUFFER(buffer) \
    if(this->mode == MODEM_MANUAL_MODE) { \
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

#define REMOVE_NON_ASCII(data) \
    for (size_t i = 0; i < data.size(); ++i) { \
        char c = data[i]; \
        if ((c < 32 && c != '\n' && c != '\r') || c >= 0x7f) { /* Remove non ASCII char */ \
            data.erase(i, 1); \
            i--; \
        } \
    }
/*  --------------------------------------------------------  */
/*  Callback for receiving data from modem */
void Terminal::update_read(Glib::ustring data)
{
    size_t iReplyOem;
    size_t iddot;
    size_t replyOemLen = strlen(this->modem->REPLY_OEM().c_str());

    if(!data.is_ascii())
        REMOVE_NON_ASCII(data);
    inputPortBuffer += data;

    while(!inputPortBuffer.empty()) {
        iReplyOem = inputPortBuffer.find(this->modem->REPLY_OEM());
        iddot     = inputPortBuffer.find(":");

        if(iReplyOem == std::string::npos && iddot == std::string::npos) 
            return;

        if(iReplyOem < iddot) {
            input.push(inputPortBuffer.substr(0, iReplyOem));
            inputPortBuffer.erase(0, iReplyOem + replyOemLen);
        }
        else {
            input.push(inputPortBuffer.substr(0, iddot+1)); /* 1 == strlen(":") */
            inputPortBuffer.erase(0, iddot + 1);            /* 1 == strlen(":") */
        }
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Write to the interface and decrease the output buffer */
void Terminal::update_write()
{
    if(this->interface != NULL && this->interface->is_open()) {
        for (unsigned int i = 0; i < this->output.size(); ++i) {
            Glib::ustring *str = new Glib::ustring(this->output.front());

            /*  It is here that the message if formated according to 
             *  the modem beeing used and it's mode */
            *str = this->modem->format_send(*str);

            this->interface->write(str);
            this->output.push(this->output.front());
            this->output.pop();
        }
    }
}
/*  --------------------------------------------------------  */

#define WRITE_TO_TEXTVIEW(buffer) \
    for (unsigned int i = 0; i < buffer.size(); i++) { \
        aux = buffer.front(); \
        buffer.pop(); \
        buff << aux << "\n"; \
        if(this->mode == MODEM_AUTO_MODE) \
        buffer.push(aux); \
    }

#define WRITE_OUTPUT_TO_TEXTVIEW(buffer) \
    for (unsigned int i = 0; i < buffer.size(); i++) { \
        aux = buffer.front(); \
        buffer.pop(); \
        buff << aux << "\n"; \
    }

#define SMART_ERASE_OUTPUT_BUFFER(buffer) \
    for (unsigned int i = 0; i < buffer.size(); i++) { \
        aux = buffer.front(); \
        buffer.pop(); \
    }

/*  --------------------------------------------------------  */
/* Puts the input and output buffer in the Gtk::TextBuffer */
void Terminal::update_buffer()
{
    if(this->input.size() > 0 || this->output.size() > 0) {
        unsigned int lines = this->buffer->get_line_count();
        Gtk::TextIter begin, end;
        std::string aux;
        std::stringstream buff;

        /* Initializes the stream string buff */
        buff << this->buffer->get_text();

        /*  Open the necessary space for the buffer */
        int count = input.size() + this->output.size() + lines - max_num_lines;
        for (int i = 0; i < count; ++i) {
            std::getline(buff, aux);

            begin = this->buffer->begin();
            end = this->buffer->get_iter_at_offset(aux.size()+1);
            this->buffer->erase(begin, end);
        }

        /*  Resets the new buffer */
        buff.str(std::string());

        /* Writes to buffer */
        WRITE_TO_TEXTVIEW(this->input);
        if(this->mode == MODEM_MANUAL_MODE) {
            SMART_ERASE_OUTPUT_BUFFER(this->output);
        }
        else {
            WRITE_OUTPUT_TO_TEXTVIEW (this->output);
        }

        end = this->buffer->end();
        this->buffer->insert(end, buff.str());
        this->buffer->apply_tag(this->notEditableTag, buffer->begin(), buffer->end());
        this->textView->scroll_to(end, (double)0);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Puts the input and output buffer in the Gtk::TextBuffer */
/*void Terminal::update_buffer()
{
    if(this->input.size() > 0 || this->output.size() > 0) {
        unsigned int lines = this->buffer->get_line_count();
        std::string str = this->buffer->get_text();
        std::string aux;
        std::stringstream buff;
        unsigned int size;
        buff << str;
*/
        /*  Open the necessary space for the buffer */
/*        int count = input.size() + this->output.size() + lines - max_num_lines;
        for (int i = 0; i < count; ++i) {
            std::getline(buff, aux);
            size = aux.size();
            str.erase(0, size+1);
        }
*/
        /*  Resets the new buffer */
//        buff.str(std::string());

 //       buff << str; /*  Assignes the new string to the buffer */

        /* Writes to buffer */
  /*        WRITE_TO_TEXTVIEW(this->input);
        if(this->mode == MODEM_MANUAL_MODE) {
            SMART_ERASE_OUTPUT_BUFFER(this->output);
        }
        else {
            WRITE_OUTPUT_TO_TEXTVIEW (this->output);
        }

        this->buffer->set_text(buff.str());
        this->buffer->apply_tag(this->notEditableTag, buffer->begin(), buffer->end());
        Gtk::TextBuffer::iterator iter = buffer->end();
        this->textView->scroll_to(iter, (double)0);
    }
}*/
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool Terminal::write_to_device(std::string str)
{
    if(mode == MODEM_AUTO_MODE) {
        this->output.push(str);
        return true;
    }

    return false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/*  Returns true if the modem status is MODEM_AUTO_MODE and 
 *  false otherwise */
bool Terminal::read_from_device(std::string *str) 
{
    if(mode == MODEM_AUTO_MODE && !is_read_locked()) {

        /* flush write buffer before waiting for input */
        this->update();

        /* Waits until an input is received or satellite goes below
         * the horizon */
        rec->wait_for_input();

        if(this->input.size() == 0) {
            *str = DATA_NOT_RECEIVED;
            this->read_locker = true; // locks read for not receiving more data;
            Log::LogWarn(LEVEL_LOG_SILENT, "DATA NOT RECEIVED FROM MODEM", __FILE__, __LINE__);
        }
        else {
            *str = this->input.front();
            this->input.pop();
        }

        return true;
    }
    else {
        *str = DATA_NOT_RECEIVED;
        Log::LogWarn(LEVEL_LOG_SILENT, "DATA NOT RECEIVED FROM MODEM BECAUSE SAT IS UNDER THE HORIZON OR A PROBLEM OCCURED", __FILE__, __LINE__);
    }

    return false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void Terminal::change_config_mode()
{
    if(this->mode == MODEM_MANUAL_MODE) {
        this->mode = MODEM_AUTO_MODE;
        Log::LogWarn(LEVEL_LOG_INFO, "Terminal mode changed to MODEM AUTO MODE sucessfully", __FILE__, __LINE__);
    }
    else {
        this->mode = MODEM_MANUAL_MODE;
        Log::LogWarn(LEVEL_LOG_INFO, "Terminal mode changed to MODEM MANUAL MODE sucessfully", __FILE__, __LINE__);
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

/*  --------------------------------------------------------  */
void Terminal::set_textview(Gtk::TextView *textView)
{
    this->textView = textView;
}
/*  --------------------------------------------------------  */

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

/*  --------------------------------------------------------  */
void Terminal::set_modem(Modem *modem)
{
    this->modem = modem;
    this->modem->set_change_modem_mode_cb(sigc::mem_fun(*this, &Terminal::modem_mode_changed_combobox_cb));
    this->modem->set_change_modem_name_cb(sigc::mem_fun(*this, &Terminal::modem_name_changed_combobox_cb));
}
/*  --------------------------------------------------------  */

void Terminal::modem_name_changed_combobox_cb()
{
    if(this->mode == MODEM_MANUAL_MODE) {
        this->modem->update_modem_name();
    }
    else {
        Log::LogWarn(LEVEL_LOG_INFO, "Change to manual mode first", __FILE__, __LINE__);
    }
}

/*  --------------------------------------------------------  */
void Terminal::modem_mode_changed_combobox_cb()
{
    if(this->mode == MODEM_MANUAL_MODE && this->interface != NULL && this->interface->is_open()) {
        Glib::ustring *change_mode_str = new Glib::ustring(this->modem->update_modem_mode());
    
        if(change_mode_str->size() > 0) {
            this->update_write();           /* Empty buffer before sending something */
            this->interface->write(change_mode_str);    /* Write to change mode str */
        }

    }
    else {
        Log::LogWarn(LEVEL_LOG_INFO, "Change to manual mode first", __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/*bool Terminal::block_read()
{
    if (this->satEl == NULL) {
        Log::LogWarn(LEVEL_LOG_WARNING, "Terminal unable to get satellite elevation, receiving the data may not work", __FILE__, __LINE__);
        return false;
    }

    Glib::ustring satEl = this->satEl->get_text();

    try {
        if (satEl.compare("Not Set") && (std::stof(satEl) >= 0.0f)) {
            return true;
        }
        else 
            return false;
    }
    catch (std::exception &e) {
        return false;
    }
}*/
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
std::queue<std::string> *Terminal::get_input_buffer()
{
    return &this->input;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void Terminal::set_gtk_receive(GtkReceive *rec)
{
    this->rec = rec;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/*  Returns true if read is locked, false otherwise. */
bool Terminal::is_read_locked()
{
    return this->read_locker;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void Terminal::unlock_read()
{
    this->read_locker = false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void Terminal::close_gtk_receive_window()
{
    this->rec->close_window();
}
/*  --------------------------------------------------------  */
