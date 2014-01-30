#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <gtkmm.h>

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
    SATS
} sats_treeview;

/*
 * Curr sat
 */
#define CURR_SAT_LABEL_WIDGET "sat"
#define STATUS_LABEL_WIDGET "status"
#define AOS_LABEL_WIDGET "AOS"
#define LOS_LABEL_WIDGET "LOS"

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

class MainWindow
{
    private:
        void init_sats();
        void init_alias();
        void init_text_editor();
        void init_curr_sat();

    public:
        Glib::RefPtr<Gtk::Builder> mainBuilder;
        Gtk::Window * mainWindow;

        Gtk::TreeView * satsTreeview;

        Gtk::Label *satName, *status, *aos, *los;
        int fifo_fd;

        Gtk::TreeView * aliasTreeview;
        std::vector<Gtk::CellRendererText*> aliasColumnCells;
        std::vector<Gtk::CellRenderer*> commandColumnCells;

        Gtk::TextView * textEditor;

        MainWindow();
        Gtk::TextView * get_textView();
        Gtk::Window * get_mainWindow();
        Glib::RefPtr<Gtk::Builder> get_mainBuilder();

        void cellrenderColumnAlias_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text);
        void cellrenderColumnCommand_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text);
        bool updateCurrSatellite();
};

#endif
