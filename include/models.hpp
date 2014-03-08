/*
 * MODELS_FILE : This file specifies the models used
 * in GTK for this program
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

#endif
