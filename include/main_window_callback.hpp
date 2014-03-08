#ifndef MAIN_WINDOW_CALLBACK_HPP
#define MAIN_WINDOW_CALLBACK_HPP

#include "main_window_renderer.hpp"
#include "in_out_interface.hpp"
#include "manager.hpp"
#include "menu.hpp"
#include "interpreter.hpp"

class MainWindowCallback
{

    private:
        /*----------------------------------------------*/
        /* General attributes */
        int fifo_fd;
        Manager *man;
        InOutInterface *inter;
        MainWindowRenderer *main_window_renderer;
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
        bool update_curr_satellite();
        bool on_key_press_event(GdkEventKey* event);
        void quit_cb();
        /*----------------------------------------------*/

    public:
        MainWindowCallback(Manager *man, InOutInterface *inter);
        Gtk::Window *get_main_window();
};
 
#endif
