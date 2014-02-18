/*
 * MODELS_FILE : This file specifies the models used
 * in GTK for this program
 */

#ifndef MODELS_HPP
#define MODELS_HPP

#include <gtkmm.h>

#include <string>

class ModelAliasColumns : public Gtk::TreeModel::ColumnRecord
{
    public:
        ModelAliasColumns()
        { add(col_alias); add(col_command); }

        Gtk::TreeModelColumn<Glib::ustring> col_alias;
        Gtk::TreeModelColumn<Glib::ustring> col_command;
};

class ModelCommandsColumns : public Gtk::TreeModel::ColumnRecord
{
    public:
        ModelCommandsColumns()
        { add(col_command_name); }

        Gtk::TreeModelColumn<Glib::ustring> col_command_name;
};

class ModelPortSpeedComboBox : public Gtk::TreeModel::ColumnRecord
{
    public:
        ModelPortSpeedComboBox()
        { add(speedName);}

        Gtk::TreeModelColumn<Glib::ustring> speedName;
};

class ModelSatsColumns : public Gtk::TreeModel::ColumnRecord
{
    public:
        ModelSatsColumns()
        { add(col_sat_name); add(col_script_name); }

        Gtk::TreeModelColumn<Glib::ustring> col_sat_name;
        Gtk::TreeModelColumn<Glib::ustring> col_script_name;
};

#endif
