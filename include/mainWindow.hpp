#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <gtkmm.h>

#include "manager.hpp"
#include "config_port_model.hpp"
#include "commands_model.hpp"

#define MAIN_WINDOW_GLADE "windows/mainWindow.glade"

#define MAIN_WINDOW_WIDGET "mainWindow"

#define FIFO_FILE "/tmp/GpredictFIFO"
#define SATS_FILE "/tmp/GpredictSATS"

#define MAX_SAT_NAME 400
#define MAX_M_SIZE   430

#define UPDATE_RATE  1000

/*
 * Menu
 */
#define SAVEAS_FILE_WIDGET "saveFileAs"
#define OPEN_FILE_WIDGET "openFile"
#define QUIT_WIDGET "quit"
#define ABOUT_FILE_WIDGET "about"

/*
 * Sats List
 */
#define SATS_TREEVIEW_WIDGET "satsTreeview"
typedef enum sats_treeview_t {
    SATS,
    SCRIPTS
} sats_treeview;

/*
 * Curr sat
 */
#define CURR_SAT_LABEL_WIDGET "sat"
#define STATUS_LABEL_WIDGET "status"
#define ELEVATION_LABEL_WIDGET "elevation"
#define AZIMUTH_LABEL_WIDGET "azimuth"

/*
 * Text Editor
 */
#define TEXT_VIEW_WIDGET "commandsFileText"

/*
 * Alias
 */
#define ALIAS_TREEVIEW_WIDGET "aliasTreeview"
typedef enum alias_treeview_t {
    ALIAS,
    COMMAND
} alias_treeview;

/*
 * Config
 */
#define CONFIG_SAT_NAME_WIDGET "satNameLabel"
#define CONFIG_SCRIPT_NAME_WIDGET "scriptNameLabel"

/*
 * Port Config
 */
#define PORT_NAME_ENTRY_WIDGET "portName"
#define PORT_SPEED_COMBOBOX_WIDGET "serialPortComboBox"
#define PORT_NAME_STATUS_WIDGET "serialPortNameStatus"
#define UPS_SPEED_STATUS_WIDGET "upsStatus"

/*
 * Commands
 */
#define COMMANDS_TREEVIEW_WIDGET "commandsTreeview"
typedef enum command_treeview_t {
    COMMANDS
} command_treeview;

class MainWindow
{
    private:
        Manager *man;
        InOutInterface *inter;
        void init_sats_frame();
        void init_alias_frame();
        void init_text_editor();
        void init_curr_sat_frame();
        void init_config_frame();
        void init_port_config_frame();
        void init_commands_frame();

        Glib::RefPtr<Gtk::TextBuffer> *textBuffer;
        Glib::RefPtr<Gtk::ListStore> *aliasModel;

        Gtk::TreeView *commandsTreeView;

        Gtk::Label *configSatNameLabel;
        Gtk::Label *configScriptNameLabel;
        
    public:
        Glib::RefPtr<Gtk::Builder> mainBuilder;
        Gtk::Window * mainWindow;

        Gtk::TreeView * satsTreeview;

        Gtk::Label *satName, *status, *satEl, *satAz;
        int fifo_fd;

        Gtk::TreeView * aliasTreeview;
        std::vector<Gtk::CellRendererText*> aliasColumnCells;
        std::vector<Gtk::CellRenderer*> commandColumnCells;

        Gtk::TextView * textEditor;

        MainWindow(Manager *man, InOutInterface *inter);
        Gtk::TextView * get_textView();
        Gtk::Window * get_mainWindow();
        Glib::RefPtr<Gtk::Builder> get_mainBuilder();

        void satsTreeView_activated_cb(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
        void cellrenderColumnAlias_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text);
        void cellrenderColumnCommand_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text);
        bool updateCurrSatellite();
        void portNameEntry_activated_cb();
        void portSpeedComboBox_changed_cb();
        void commandTreeView_activated_cb(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
};

#endif
