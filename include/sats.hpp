#ifndef SATS_HPP
#define SATS_HPP

#include <gtkmm.h>

class ModelSatsColumns : public Gtk::TreeModel::ColumnRecord
{
    public:
        ModelSatsColumns()
        { add(col_sat_name); add(col_script_name); }

        Gtk::TreeModelColumn<Glib::ustring> col_sat_name;
        Gtk::TreeModelColumn<Glib::ustring> col_script_name;
};

#endif
