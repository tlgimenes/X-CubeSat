#include <gtkmm.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "mainWindow.hpp"
#include "log.hpp"

void MainWindow::init_sats_frame()
{ 
    this->mainBuilder->get_widget(SATS_TREEVIEW_WIDGET, this->satsTreeview);
    Glib::RefPtr<Gtk::ListStore> model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(this->satsTreeview->get_model());

    this->satsTreeview->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindow::satsTreeView_activated_cb));
    // For Gtk+3.8 Only
    //this->satsTreeview->set_activate_on_single_click(true); 

    this->satsTreeview->set_model(*this->man->GetModelSatsStore());
}

void MainWindow::init_curr_sat_frame()
{ 
    this->mainBuilder->get_widget(CURR_SAT_LABEL_WIDGET, this->satName);
    this->mainBuilder->get_widget(STATUS_LABEL_WIDGET, this->status);
    this->mainBuilder->get_widget(ELEVATION_LABEL_WIDGET, this->satEl);
    this->mainBuilder->get_widget(AZIMUTH_LABEL_WIDGET, this->satAz);

    this->fifo_fd = open(FIFO_FILE, O_RDWR | O_ASYNC | O_NONBLOCK);
    if(fifo_fd == -1)
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to load Gpredict data, the program will be closed !", __FILE__, __LINE__);

    // Time out to able to update sats name
    sigc::slot<bool> my_slot = sigc::mem_fun(this,&MainWindow::updateCurrSatellite);//, m_timer_number);

      // This is where we connect the slot to the Glib::signal_timeout()
    Glib::signal_timeout().connect(my_slot, UPDATE_RATE);
}

void MainWindow::init_alias_frame()
{
    this->mainBuilder->get_widget(ALIAS_TREEVIEW_WIDGET, this->aliasTreeview);

    std::vector<Gtk::CellRenderer*> renderer = this->aliasTreeview->get_column(ALIAS)->get_cells();
    ((Gtk::CellRendererText*)(renderer[0]))->signal_edited().connect(sigc::mem_fun(this, &MainWindow::cellrenderColumnAlias_edited_cb));
    ((Gtk::CellRendererText*)(renderer[0]))->property_editable().set_value(true);

    renderer = this->aliasTreeview->get_column(COMMAND)->get_cells();
    ((Gtk::CellRendererText*)(renderer[0]))->signal_edited().connect(sigc::mem_fun(this, &MainWindow::cellrenderColumnCommand_edited_cb));
    ((Gtk::CellRendererText*)(renderer[0]))->property_editable().set_value(true);

}

void MainWindow::init_config_frame()
{
    this->mainBuilder->get_widget(CONFIG_SAT_NAME_WIDGET, this->configSatNameLabel);

    this->mainBuilder->get_widget(CONFIG_SCRIPT_NAME_WIDGET, this->configScriptNameLabel);
}

void MainWindow::init_text_editor()
{
    this->mainBuilder->get_widget(TEXT_VIEW_WIDGET, this->textEditor);
}

void MainWindow::init_port_config_frame()
{
    Gtk::Entry *portName;
    Gtk::Image *portNameStatus;
    Gtk::ComboBox *portSpeedComboBox;
    Gtk::Image *portSpeedStatus;

    this->mainBuilder->get_widget(PORT_NAME_ENTRY_WIDGET, portName);
    this->mainBuilder->get_widget(PORT_NAME_STATUS_WIDGET, portNameStatus);
    this->mainBuilder->get_widget(PORT_SPEED_COMBOBOX_WIDGET, portSpeedComboBox);
    this->mainBuilder->get_widget(UPS_SPEED_STATUS_WIDGET, portSpeedStatus);

    this->inter->InitConfigFrameGtk(portName, portNameStatus, portSpeedComboBox, portSpeedStatus);
}

void MainWindow::init_commands_frame()
{
    this->mainBuilder->get_widget(COMMANDS_TREEVIEW_WIDGET, this->commandsTreeView);

    this->commandsTreeView->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindow::commandTreeView_activated_cb));
}

MainWindow::MainWindow(Manager *man, InOutInterface *inter)
{
    try {
        this->inter = inter;
        this->man = man;
        this->mainBuilder = Gtk::Builder::create_from_file(MAIN_WINDOW_GLADE);
        this->mainBuilder->get_widget(MAIN_WINDOW_WIDGET, this->mainWindow);

        /* Config */
        init_config_frame();

        /* Sats */
        init_sats_frame();

        /* Alias */
        init_alias_frame();
        
        /* Text */
        init_text_editor();

        /* Current Sat */
        init_curr_sat_frame();

        /* Port Config */
        init_port_config_frame();

        /* Commands */
        init_commands_frame();
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

void MainWindow::satsTreeView_activated_cb(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
    Glib::ustring scriptName;
    Glib::ustring satName;
    Gtk::TreeStore::Row currRow = *((this->satsTreeview->get_model()->get_iter(path)));

    Gtk::TreeStore::iterator par = currRow.parent();
    if(par) {
        (*(par)).get_value(SATS, satName);
        currRow.get_value(SCRIPTS, scriptName);

        this->aliasModel = this->man->GetModelAliasList(satName, scriptName);
        this->aliasTreeview->set_model(*this->aliasModel);

        this->textBuffer = this->man->GetTextBuffer(satName, scriptName);
        this->textEditor->set_buffer(*this->textBuffer);

        this->configSatNameLabel->set_text(satName);
        this->configScriptNameLabel->set_text(scriptName);
    }
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

void MainWindow::commandTreeView_activated_cb(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
    Glib::ustring satName = this->configSatNameLabel->get_text();
    Glib::ustring scriptName = this->configScriptNameLabel->get_text();

    if(this->man->existsSat(&satName)) {
        Glib::RefPtr<Gtk::TextBuffer> *buff = this->man->GetTextBuffer(satName, scriptName);
        
        Glib::RefPtr<Gtk::ListStore> model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(this->commandsTreeView->get_model());
    
        ModelCommandsColumns mcc;

        Glib::ustring command = model->get_iter(path)->get_value(mcc.col_command_name);
    
        (*buff)->insert_at_cursor(command);
    }
    else {
        Log::LogWarn(LEVEL_LOG_INFO, "Open a script associated with a satellite first", __FILE__, __LINE__);
    }
}

char * read_fifo_format(int fifo_fd)
{ 
    char rawIn[MAX_M_SIZE] = "0";
    char * satInfo = NULL;
    int rawIn_size = 0, it = 1, it2 = 1;

    // Reads the fifo file
    if(read(fifo_fd, rawIn, 1) == -1) return NULL;
    for(; rawIn[0]-'0' < 10 && rawIn[0]-'0' >= 0; it*=10) {
        rawIn_size = rawIn_size * it + (rawIn[0]-'0');
        if(read(fifo_fd, rawIn, 1) == -1) return NULL;
        it2++;
    }
    rawIn_size = rawIn_size - it2;

    satInfo = (char *)malloc(sizeof(char)*rawIn_size);

    if(read(fifo_fd, satInfo, rawIn_size) == -1) return NULL;

    satInfo[rawIn_size] = '\0';

    return satInfo;
}

bool MainWindow::updateCurrSatellite()
{
    char *satName, *satEl, *satAz;

    satName = read_fifo_format(this->fifo_fd);
    if(satName == NULL) satName = (char*)"Not Found";
    Glib::ustring satNameStr = satName;
    this->satName->set_text(satNameStr);
    
    satEl = read_fifo_format(this->fifo_fd);
    if(satEl == NULL) satEl = (char*)"Not Set";
    Glib::ustring satElStr = satEl;
    this->satEl->set_text(satEl);

    satAz = read_fifo_format(this->fifo_fd);
    if(satAz == NULL) satAz = (char*)"Not Set";
    Glib::ustring satAzStr = satAz;
    this->satAz->set_text(satAz);

    return true;
}
