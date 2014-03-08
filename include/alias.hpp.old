#ifndef ALIAS_HPP
#define ALIAS_HPP

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

#endif
