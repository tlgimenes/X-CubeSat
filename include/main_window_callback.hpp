#ifndef MAIN_WINDOW_CALLBACK_HPP
#define MAIN_WINDOW_CALLBACK_HPP

#include "main_window_renderer.hpp"
#include "in_out_interface.hpp"
#include "manager.hpp"

class MainWindowCallback
{

    private:
        int fifo_fd;
        Manager *man;
        InOutInterface *inter;
        MainWindowRenderer *main_window_renderer;

        /* MODELS */
        ModelAliasColumns modelAliasColumns;
        ModelCommandsColumns modelCommandsColumns;
        ModelPortSpeedComboBox modelPortSpeedComboBox;
        ModelSatsColumns modelSatsColumns;

        void connect_callbacks();
            
        void sats_treeview_activated_cb(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
        void cellrender_column_alias_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text);
        void cellrender_column_command_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text);
        void portNameEntry_activated_cb();
        void portSpeedComboBox_changed_cb();
        void command_treeview_activated_cb(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
        bool update_curr_satellite();

    public:
        MainWindowCallback(Manager *man, InOutInterface *inter);

};
 
#endif
