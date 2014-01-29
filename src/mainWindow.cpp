#include <gtkmm.h>
#include <iostream>

#include "mainWindow.hpp"
#include "log.hpp"

MainWindow::MainWindow()
{
    try {
        this->mainBuilder = Gtk::Builder::create_from_file(MAIN_WINDOW_GLADE);
        this->mainBuilder->get_widget(MAIN_WINDOW_WIDGET, this->mainWindow);

        /*
         * Alias
         */
        this->mainBuilder->get_widget(ALIAS_TREEVIEW_WIDGET, this->aliasTreeview);
        std::vector<Gtk::CellRenderer*> aux = this->aliasTreeview->get_column(ALIAS)->get_cells();
        for(Gtk::CellRenderer* cell : aux) {
            this->aliasColumnCells.push_back((Gtk::CellRendererText*)cell);
            ((Gtk::CellRendererText*)cell)->signal_edited().connect(sigc::mem_fun(this, &MainWindow::cellrenderColumnAlias_edited_cb));
            ((Gtk::CellRendererText*)cell)->property_editable_set();
            ((Gtk::CellRendererText*)cell)->property_editable().set_value(true);
        }
        std::vector<Gtk::CellRenderer*> aux2 = this->aliasTreeview->get_column(COMMAND)->get_cells();
        for(Gtk::CellRenderer * cell : aux2) {
            this->commandColumnCells.push_back((Gtk::CellRendererText*)cell);
            ((Gtk::CellRendererText*)cell)->signal_edited().connect(sigc::mem_fun(this, &MainWindow::cellrenderColumnCommand_edited_cb));
            ((Gtk::CellRendererText*)cell)->property_editable().set_value(true);
        }

        /*
         * Text
         */
        this->mainBuilder->get_widget(TEXT_VIEW_WIDGET, this->textEditor);
    }
    catch(const Glib::FileError& ex) {
        Log::LogWarn(LEVEL_LOG_WARNING, ex.what().c_str(), __FILE__, __LINE__);
    }
};

void Gtk::CellRendererText::on_edited(const Glib::ustring &path, const Glib::ustring &   new_text)
{
    std::cout << "voila" << std::endl;
}

Gtk::TextView * MainWindow::get_textView()
{
    return this->textEditor;
}

Gtk::Window * MainWindow::get_mainWindow()
{
    return this->mainWindow;
}

Glib::RefPtr<Gtk::Builder> MainWindow::get_mainBuilder()
{
    return this->mainBuilder;
}

void MainWindow::cellrenderColumnAlias_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text) 
{

}

void MainWindow::cellrenderColumnCommand_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text)
{

}
