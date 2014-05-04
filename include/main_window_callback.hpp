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

#ifndef MAIN_WINDOW_CALLBACK_HPP
#define MAIN_WINDOW_CALLBACK_HPP

#include "main_window_renderer.hpp"
#include "in_out_interface.hpp"
#include "manager.hpp"
#include "menu.hpp"
#include "xcubesat_interpreter.hpp"
#include "interpreter.hpp"

class MainWindowCallback
{
    private:
        /*----------------------------------------------*/
        /* General attributes */
        Manager *man;
        Terminal *term;
        MainWindowRenderer *main_window_renderer;
        /*----------------------------------------------*/

        /*----------------------------------------------*/
        /* Timeout callback attributes */
        bool isRunning;
        /*----------------------------------------------*/

        /*----------------------------------------------*/
        /* FIFO file for communication with Gpredict */
        std::ifstream *fifo;
        /*----------------------------------------------*/

        /*----------------------------------------------*/
        /* MODELS */
        ModelAliasColumns modelAliasColumns;
        ModelCommandsColumns modelCommandsColumns;
        ModelPortSpeedComboBox modelPortSpeedComboBox;
        ModelSatsColumns modelSatsColumns;
        ModelScriptsPriorityQueue modelScriptsPriorityQueue;
        /*----------------------------------------------*/

        /*----------------------------------------------*/
        /* Connect the call backs */
        void connect_callbacks();
        /*----------------------------------------------*/
            
        /*----------------------------------------------*/
        /* Call backs functions */
        void cellrender_column_scripts_name_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text);
        void sats_treeview_activated_cb(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
        void cellrender_column_alias_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text);
        void cellrender_column_command_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text);
        void device_name_entry_activated_cb();
        void device_speed_combobox_changed_cb();
        void command_treeview_activated_cb(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
        void new_script_button_clicked_cb();
        void up_button_clicked_cb();
        void down_button_clicked_cb();
        void update_curr_satellite();
        bool on_key_press_event(GdkEventKey* event);
        void quit_cb();
        bool quit_all_cb(GdkEventAny *event);
        void on_response_save_message_dialog(int response_id);
        bool timeout_cb();
        void on_modem_mode_change_cb();
        /*----------------------------------------------*/

    public:
        MainWindowCallback(Manager *man, Terminal *term);
        Gtk::Window *get_main_window();
};
 
#endif
