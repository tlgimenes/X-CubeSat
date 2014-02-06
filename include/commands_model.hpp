#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <gtkmm.h>

class ModelCommandsColumns : public Gtk::TreeModel::ColumnRecord
{
    public:
        ModelCommandsColumns()
        { add(col_command_name); }

        Gtk::TreeModelColumn<Glib::ustring> col_command_name;
};

#endif
