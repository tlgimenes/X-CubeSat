#include <gtkmm.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "mainWindow.hpp"
#include "log.hpp"

void MainWindow::init_sats()
{ 
    this->mainBuilder->get_widget(SATS_TREEVIEW_WIDGET, this->satsTreeview);
    Glib::RefPtr<Gtk::ListStore> model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(this->satsTreeview->get_model());

    std::ifstream sats_list(SATS_FILE);
    std::string sat_nickname;

    while(!std::getline (sats_list, sat_nickname).eof()) {
        Gtk::TreeModel::Row newRow = *(model->append());
        newRow.set_value(SATS, sat_nickname);
    }
}

void MainWindow::init_curr_sat()
{ 
    this->mainBuilder->get_widget(CURR_SAT_LABEL_WIDGET, this->satName);
    this->mainBuilder->get_widget(STATUS_LABEL_WIDGET, this->status);
    this->mainBuilder->get_widget(AOS_LABEL_WIDGET, this->aos);
    this->mainBuilder->get_widget(LOS_LABEL_WIDGET, this->los);

    this->fifo_fd = open(FIFO_FILE, O_RDWR | O_ASYNC | O_NONBLOCK);
    if(fifo_fd == -1)
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to load Gpredict data, the program will be closed !", __FILE__, __LINE__);

    // Time out to able to update sats name
    sigc::slot<bool> my_slot = sigc::mem_fun(this,&MainWindow::updateCurrSatellite);//, m_timer_number);

      // This is where we connect the slot to the Glib::signal_timeout()
    Glib::signal_timeout().connect(my_slot, UPDATE_RATE);
}

void MainWindow::init_alias()
{
    this->mainBuilder->get_widget(ALIAS_TREEVIEW_WIDGET, this->aliasTreeview);

    std::vector<Gtk::CellRenderer*> renderer = this->aliasTreeview->get_column(ALIAS)->get_cells();
    ((Gtk::CellRendererText*)(renderer[0]))->signal_edited().connect(sigc::mem_fun(this, &MainWindow::cellrenderColumnAlias_edited_cb));
    ((Gtk::CellRendererText*)(renderer[0]))->property_editable().set_value(true);

    renderer = this->aliasTreeview->get_column(COMMAND)->get_cells();
    ((Gtk::CellRendererText*)(renderer[0]))->signal_edited().connect(sigc::mem_fun(this, &MainWindow::cellrenderColumnCommand_edited_cb));
    ((Gtk::CellRendererText*)(renderer[0]))->property_editable().set_value(true);

}

void MainWindow::init_text_editor()
{
    this->mainBuilder->get_widget(TEXT_VIEW_WIDGET, this->textEditor);
}

MainWindow::MainWindow()
{
    try {
        this->mainBuilder = Gtk::Builder::create_from_file(MAIN_WINDOW_GLADE);
        this->mainBuilder->get_widget(MAIN_WINDOW_WIDGET, this->mainWindow);

        /* Sats */
        init_sats();

        /* Current Sat */
        init_curr_sat();

        /* Alias */
        init_alias();

        /* Text */
        init_text_editor();
    }
    catch(const Glib::FileError& ex) {
        Log::LogWarn(LEVEL_LOG_WARNING, ex.what().c_str(), __FILE__, __LINE__);
    }
};

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
    Gtk::TreePath treePath(path);
    Glib::RefPtr<Gtk::ListStore> model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(this->aliasTreeview->get_model());
    
    // Sets new string to the cell box
    model->get_iter(treePath)->set_value(ALIAS, new_text);
    
}

void MainWindow::cellrenderColumnCommand_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text)
{
    Glib::ustring new_alias("new_alias");
    Glib::ustring new_command("new_command");
    Gtk::TreePath treePath(path);
    Glib::RefPtr<Gtk::ListStore> model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(this->aliasTreeview->get_model());
    Gtk::ListStore::iterator curr = model->get_iter(treePath);
    Gtk::TreeModel::Row currRow = *(curr);
    Glib::ustring old_text;

    currRow.get_value(COMMAND, old_text);

    // If we are adding a new command
    // than add new row
    if(!old_text.compare(new_command)) {
        Gtk::TreeModel::Row newRow = *(model->append());
        newRow.set_value(ALIAS, new_alias);
        newRow.set_value(COMMAND, new_command);
    }

    // Sets new string to the cell box
    curr->set_value(COMMAND, new_text);
    
}

bool MainWindow::updateCurrSatellite()
{
    char rawIn[MAX_M_SIZE] = "0";
    char satName[MAX_SAT_NAME] = "N";
    int rawIn_size = 0, it = 1, it2 = 0;
    float aos, los;
    std::stringstream itos;

    /*// Reads the fifo file
    read(this->fifo_fd, rawIn, 1);
    for(; rawIn[0]-'0' < 10 && rawIn[0]-'0' >= 0; it*=10) {
        rawIn_size = rawIn_size * it + (rawIn[0]-'0');
        read(this->fifo_fd, rawIn, 1);
        it2++;
    }
    std::cout << "size: " << rawIn_size << std::endl;
    rawIn_size-=it2-1;

    read(this->fifo_fd, rawIn, rawIn_size);

 //   read(this->fifo_fd, rawIn, 37);

    std::cout << "rawIn: " << rawIn << " " << sscanf(rawIn, "%s %f %f\n", satName, &aos, &los) << std::endl;
    */
    read(this->fifo_fd, satName, MAX_SAT_NAME);

    Glib::ustring satNameStr = satName;
    this->satName->set_text(satNameStr);
    
    itos << aos;
    this->aos->set_text(itos.str());
    itos << los;
    this->los->set_text(itos.str());

    return true;
}
