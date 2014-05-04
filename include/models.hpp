/*
 * MODELS_FILE : This file specifies the models used
 * in GTK for this program
 */
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

#ifndef MODELS_HPP
#define MODELS_HPP

#include <gtkmm.h>

#include <string>

/* --------------------------------------------------------------*/
/*
 *  Model for the sattelites treeview 
 */
typedef enum sats_treeview_t {
    SATS,
    SCRIPTS
} sats_treeview;

class ModelSatsColumns : public Gtk::TreeModel::ColumnRecord
{
    public:
        ModelSatsColumns()
        { add(col_sat_name); add(col_script_name); }

        Gtk::TreeModelColumn<Glib::ustring> col_sat_name;
        Gtk::TreeModelColumn<Glib::ustring> col_script_name;

        sats_treeview col_sat_name_int = SATS;
        sats_treeview col_script_name_int = SCRIPTS;
};
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
/*
 *  Model for the alias treeview
 */
typedef enum alias_treeview_t {
    ALIAS,
    COMMAND
} alias_treeview;

class ModelAliasColumns : public Gtk::TreeModel::ColumnRecord
{
    public:
        ModelAliasColumns()
        { add(col_alias); add(col_command); }

        Gtk::TreeModelColumn<Glib::ustring> col_alias;
        Gtk::TreeModelColumn<Glib::ustring> col_command;

        alias_treeview col_alias_int = ALIAS;
        alias_treeview col_command_int = COMMAND;
};
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
/*
 *  Model the commands treeview
 */
typedef enum command_treeview_t {
    COMMANDS
} command_treeview;

class ModelCommandsColumns : public Gtk::TreeModel::ColumnRecord
{
    public:
        ModelCommandsColumns()
        { add(col_command_name); }

        Gtk::TreeModelColumn<Glib::ustring> col_command_name;

        command_treeview_t col_command_name_int = COMMANDS;
};

/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
/*
 *  Model for the speed combobox of the input/output interface
 */
class ModelPortSpeedComboBox : public Gtk::TreeModel::ColumnRecord
{
    public:
        ModelPortSpeedComboBox()
        { add(speedName);}

        Gtk::TreeModelColumn<Glib::ustring> speedName;
};
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
typedef enum scripts_name_treeview_t {
    SCRIPTS_NAME
} scripts_name_treeview;

/* Model for the priority queue of scripts */
class ModelScriptsPriorityQueue : public Gtk::TreeModel::ColumnRecord
{
    public:
        ModelScriptsPriorityQueue()
        { add(col_script_name); }

        Gtk::TreeModelColumn<Glib::ustring> col_script_name;

        scripts_name_treeview col_script_name_int = SCRIPTS_NAME;
};
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
/* FIFO_FILE Model 
 * SAT_NAME
 * SAT_EL
 * SAT_AZ 
*/
typedef struct fifo_file_model_t
{
    std::string *satName;
    std::string *el;
    std::string *az;
} fifo_file_model;
/* --------------------------------------------------------------*/

/* --------------------------------------------------------------*/
/* Model for the Config port combobox */
class PortSpeedComboBoxModel : public Gtk::TreeModel::ColumnRecord
{
    public:
        PortSpeedComboBoxModel()
        { add(speedName);}

        Gtk::TreeModelColumn<Glib::ustring> speedName;
};
/* --------------------------------------------------------------*/
#endif
