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

#ifndef MAIN_WINDOW_RENDERER_HPP
#define MAIN_WINDOW_RENDERER_HPP

#include <gtkmm.h>

#include "manager.hpp"
#include "models.hpp"
#include "defs.hpp"

class MainWindowRenderer
{
    friend class MainWindowCallback;

    private:
        Glib::RefPtr<Gtk::Builder> mainBuilder;
        Gtk::Window * mainWindow;
        /*----------------------------------------------*/
        /* MODELS */
        ModelAliasColumns modelAliasColumns;
        ModelCommandsColumns modelCommandsColumns;
        ModelPortSpeedComboBox modelPortSpeedComboBox;
        ModelSatsColumns modelSatsColumns;
        /*----------------------------------------------*/

        /*----------------------------------------------*/
        /* In/Out interface attributes */
        Gtk::Entry *deviceName;
        Gtk::Image *deviceNameStatus;
        Gtk::ComboBox *deviceSpeedComboBox;
        Gtk::Image *deviceSpeedStatus;
        /*----------------------------------------------*/

        /*----------------------------------------------*/
        /* Alias treeview attributes */
        std::vector<Gtk::CellRenderer*>     aliasAliasColumnRenderer;
        std::vector<Gtk::CellRenderer*>     commandsAliasColumnRenderer;
        std::vector<Gtk::CellRenderer*>     commandColumnCells;
        std::vector<Gtk::CellRendererText*> aliasColumnCells;
        Glib::RefPtr<Gtk::ListStore>        *aliasModel;
        Gtk::TreeView                       *aliasTreeview;
        /*----------------------------------------------*/

        /*----------------------------------------------*/
        /* Text editor attributes */
        Glib::RefPtr<Gtk::TextBuffer> *textBuffer;
        Gtk::TextView * textEditor;
        /*----------------------------------------------*/

        /*----------------------------------------------*/
        /* Commands treeview attributes */
        Gtk::TreeView *commandsTreeView;
        /*----------------------------------------------*/

        /*----------------------------------------------*/
        /* Configuration frame attributes */
        Gtk::Label *configSatNameLabel;
        Gtk::Label *configScriptNameLabel;
        /*----------------------------------------------*/

        /*----------------------------------------------*/
        /* CurrSat frame attributes */
        Gtk::Label *satName, *status, *satEl, *satAz;
        /*----------------------------------------------*/

        /*----------------------------------------------*/
        /* sats treeview attributes */
        Gtk::TreeView *satsTreeview;
        /*----------------------------------------------*/

        /*----------------------------------------------*/
        /* scripts frame attributes */
        Gtk::Button *newScriptButton, *upButton, *downButton;
        Gtk::TreeView *scriptsExeQueueTreeview;
        /*----------------------------------------------*/

    protected:
        /*----------------------------------------------*/
        /* Init functions */
        void init_sats_frame(Manager *man);
        void init_alias_frame();
        void init_text_editor();
        void init_curr_sat_frame();
        void init_config_frame();
        void init_port_config_frame(InOutInterface *inter);
        void init_commands_frame();
        void init_scripts_frame();
        /*----------------------------------------------*/
  
        /*----------------------------------------------*/
        /* GET and SET functions - They are important to 
         * use because they are attribute safe (they raise
         * an error log instance in case of failure */
        Glib::RefPtr<Gtk::ListStore> get_model_treeview_alias();
        Glib::RefPtr<Gtk::TreeStore> get_model_treeview_sats();
        Glib::RefPtr<Gtk::ListStore> get_model_treeview_commands();
        Gtk::ListStore::Row          get_row_treeview_alias(Gtk::TreePath path);
        Gtk::TreeStore::Row          get_row_treeview_sats(Gtk::TreePath path);
        Gtk::ListStore::Row          get_row_treeview_commands(Gtk::TreePath path);
        Gtk::TreeModel::Row          get_row_combobox_device_active();
        Glib::ustring                get_device_name();
        Glib::ustring                get_config_sat_name();
        Glib::ustring                get_config_script_name();
        /*----------------------------------------------*/

        /*----------------------------------------------*/
        /* Render functions */
        void render_curr_sat_el_refresh(Glib::ustring);
        void render_curr_sat_az_refresh(Glib::ustring);
        void render_curr_sat_name_refresh(Glib::ustring);
        void render_curr_status_refresh(Glib::ustring);
        void render_new_alias_row(Glib::ustring aliasName, Glib::ustring commandName);
        void render_alias_column_refresh(Gtk::ListStore::iterator it, const Glib::ustring& new_text, Gtk::TreeModelColumn<Glib::ustring> column);
        void render_new_scritp_name(Glib::ustring);
        void render_new_sat_name(Glib::ustring);
        void render_new_text_editor(Glib::RefPtr<Gtk::TextBuffer> *textBuffer);
        void render_text_editor_insert_command(Glib::ustring);
        void render_new_alias(Glib::RefPtr<Gtk::ListStore> *aliasModel);
        void render_sats_list_refresh(Manager *man);
        void render_scripts_priority_queue(Glib::RefPtr<Gtk::ListStore> *Pqueue);
        /*----------------------------------------------*/
    public:
        MainWindowRenderer(Manager *man, InOutInterface *inter);
                      
        /*----------------------------------------------*/
        Glib::RefPtr<Gtk::Builder> get_main_builder();
        Gtk::TextView              *get_text_editor();
        Gtk::Window                *get_main_window();
        /*----------------------------------------------*/
};

#endif
