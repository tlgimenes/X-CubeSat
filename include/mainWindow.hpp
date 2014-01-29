#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <gtkmm.h>

#define MAIN_WINDOW_GLADE "windows/mainWindow.glade"

#define MAIN_WINDOW_WIDGET "mainWindow"

/*
 * Menu
 */
#define SAVEAS_FILE_WIDGET "saveFileAs"
#define OPEN_FILE_WIDGET "openFile"
#define QUIT_WIDGET "quit"
#define ABOUT_FILE_WIDGET "about"

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
    public:
        Glib::RefPtr<Gtk::Builder> mainBuilder;
        Gtk::Window * mainWindow;

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
};

#endif
