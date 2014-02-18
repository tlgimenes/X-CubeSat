#include <gtkmm.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "main_window_renderer.hpp"
#include "log.hpp"

void MainWindowRenderer::init_sats_frame()
{ 
    this->mainBuilder->get_widget(SATS_TREEVIEW_WIDGET, this->satsTreeview);
    Glib::RefPtr<Gtk::ListStore> model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(this->satsTreeview->get_model());

    /*this->satsTreeview->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindowRenderer::satsTreeView_activated_cb));*/
    // For Gtk+3.8 Only
    //this->satsTreeview->set_activate_on_single_click(true); 

    this->satsTreeview->set_model(*this->man->GetModelSatsStore());
}

void MainWindowRenderer::init_curr_sat_frame()
{ 
    this->mainBuilder->get_widget(CURR_SAT_LABEL_WIDGET, this->satName);
    this->mainBuilder->get_widget(STATUS_LABEL_WIDGET, this->status);
    this->mainBuilder->get_widget(ELEVATION_LABEL_WIDGET, this->satEl);
    this->mainBuilder->get_widget(AZIMUTH_LABEL_WIDGET, this->satAz);

    this->fifo_fd = open(FIFO_FILE, O_RDWR | O_ASYNC | O_NONBLOCK);
    if(fifo_fd == -1)
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to load Gpredict data, the program will be closed !", __FILE__, __LINE__);

    /*
    // Time out to able to update sats name
    sigc::slot<bool> my_slot = sigc::mem_fun(this,&MainWindowRenderer::updateCurrSatellite);//, m_timer_number);

      // This is where we connect the slot to the Glib::signal_timeout()
    Glib::signal_timeout().connect(my_slot, UPDATE_RATE);*/
}

void MainWindowRenderer::init_alias_frame()
{
    this->mainBuilder->get_widget(ALIAS_TREEVIEW_WIDGET, this->aliasTreeview);

    this->aliasAliasColumnRenderer = this->aliasTreeview->get_column(ALIAS)->get_cells();
    /*((Gtk::CellRendererText*)(renderer[0]))->signal_edited().connect(sigc::mem_fun(this, &MainWindowRenderer::cellrenderColumnAlias_edited_cb));*/
    ((Gtk::CellRendererText*)(this->aliasAliasColumnRenderer[0]))->property_editable().set_value(true);

    this->commandsAliasColumnRenderer = this->aliasTreeview->get_column(COMMAND)->get_cells();
 /*   ((Gtk::CellRendererText*)(renderer[0]))->signal_edited().connect(sigc::mem_fun(this, &MainWindowRenderer::cellrenderColumnCommand_edited_cb));*/
    ((Gtk::CellRendererText*)(this->commandsAliasColumnRenderer[0]))->property_editable().set_value(true);

}

void MainWindowRenderer::init_config_frame()
{
    this->mainBuilder->get_widget(CONFIG_SAT_NAME_WIDGET, this->configSatNameLabel);

    this->mainBuilder->get_widget(CONFIG_SCRIPT_NAME_WIDGET, this->configScriptNameLabel);
}

void MainWindowRenderer::init_text_editor()
{
    this->mainBuilder->get_widget(TEXT_VIEW_WIDGET, this->textEditor);
}

void MainWindowRenderer::init_port_config_frame()
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

void MainWindowRenderer::init_commands_frame()
{
    this->mainBuilder->get_widget(COMMANDS_TREEVIEW_WIDGET, this->commandsTreeView);

 /*   this->commandsTreeView->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindowRenderer::commandTreeView_activated_cb));*/
}

MainWindowRenderer::MainWindowRenderer(Manager *man, InOutInterface *inter)
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

Gtk::TextView * MainWindowRenderer::get_textView()
{
    return this->textEditor;
}

Gtk::Window * MainWindowRenderer::get_mainWindow()
{
    return this->mainWindow;
}

Glib::RefPtr<Gtk::Builder> MainWindowRenderer::get_mainBuilder()
{
    return this->mainBuilder;
}

void MainWindowRenderer::satsTreeView_activated_cb(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
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

void MainWindowRenderer::cellrenderColumnAlias_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text) 
{
    Gtk::TreePath treePath(path);
    Glib::RefPtr<Gtk::ListStore> model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(this->aliasTreeview->get_model());
    
    // Sets new string to the cell box
    model->get_iter(treePath)->set_value(ALIAS, new_text);
    
}

void MainWindowRenderer::cellrenderColumnCommand_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text)
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

void MainWindowRenderer::commandTreeView_activated_cb(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
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

/*
 *
 *
 *
 *
 * NEW FUNCTIONS FOR NEW ARCHTECHTURE
 *
 *
 *
 *
 *
 */

/*
#define RENDER_CURR_REFRESH(NAME, PARAM) \
void MainWindowRenderer::render_curr_NAME_refresh(Glib::ustring data) \
{ \
    if(data.size() > 0) { \
        this->PARAM->set_text(data); \
    } \
}

RENDER_CURR_REFRESH(sat_el, satEl);
RENDER_CURR_REFRESH(sat_az, satAz);
RENDER_CURR_REFRESH(sat_name, satName);
*/
void MainWindowRenderer::render_curr_sat_el_refresh(Glib::ustring data)
{
    if(data.size() > 0) {
        this->satEl->set_text(data);
    }
}
void MainWindowRenderer::render_curr_sat_az_refresh(Glib::ustring data)
{
    if(data.size() > 0) {
        this->satAz->set_text(data);
    }
}
void MainWindowRenderer::render_curr_sat_name_refresh(Glib::ustring data)
{
    if(data.size() > 0) {
        this->satName->set_text(data);
    }
}

void MainWindowRenderer::render_new_alias_row(Glib::ustring aliasName, Glib::ustring commandName)
{
    Gtk::TreeModel::Row newRow = *this->get_model_treeview_alias()->append();

    if(newRow) {
        newRow.set_value(this->modelAliasColumns.col_alias, aliasName);
        newRow.set_value(this->modelAliasColumns.col_command, commandName);

        return;
    }

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to attach a new alias", __FILE__, __LINE__);
}

void MainWindowRenderer::render_alias_column_refresh(Gtk::ListStore::iterator it, const Glib::ustring& new_text, Gtk::TreeModelColumn<Glib::ustring> column)
{
    if(new_text.size() > 0 && it) {
        // Sets new string to the cell box
        it->set_value(column, new_text);

        return;
    }

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to set refresh the alias column", __FILE__, __LINE__);
}

void MainWindowRenderer::render_new_scritp_name(Glib::ustring scriptName)
{
    if(scriptName.size() > 0) {
        this->configScriptNameLabel->set_text(scriptName);

        return;
    }

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to render the scritp name", __FILE__, __LINE__);
}

void MainWindowRenderer::render_new_sat_name(Glib::ustring satName) 
{
    if(satName.size() > 0) {
        this->configSatNameLabel->set_text(satName);

        return;
    }

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to render the satellite name", __FILE__, __LINE__);

}

void MainWindowRenderer::render_new_text_editor(Glib::RefPtr<Gtk::TextBuffer> *textBuffer)
{
    if(*textBuffer) {
        this->textBuffer = textBuffer;

        this->textEditor->set_buffer(*textBuffer);

        return;
    }

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to load scritp to text editor", __FILE__, __LINE__);
}

void MainWindowRenderer::render_new_alias(Glib::RefPtr<Gtk::ListStore> *aliasModel)
{
    if(*aliasModel) {
        this->aliasModel = aliasModel;

        this->aliasTreeview->set_model(*aliasModel);

        return;
    }

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to set the new alias model", __FILE__, __LINE__);
}

/* TODO:
 * Add tags and fancy stuff
 */
void MainWindowRenderer::render_text_editor_insert_command(Glib::ustring command)
{
    if(this->textBuffer) {
        (*this->textBuffer)->insert_at_cursor(command);

        return;
    }

    Log::LogWarn(LEVEL_LOG_INFO, "Error inserting command to script", __FILE__, __LINE__);
}

/*
#define GET_MODEL_TREEVIEW(NAME, PARAM, CLASS) \
Glib::RefPtr<CLASS> MainWindowRenderer::get_model_treeview_NAME() \
{ \
    Glib::RefPtr<CLASS> model = Glib::RefPtr<CLASS>::cast_dynamic(this->PARAM->get_model()); \
\
    if(model) \
        return model; \
\
    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get the model", __FILE__, __LINE__); \
}

GET_MODEL_TREEVIEW(alias, aliasTreeview, Gtk::ListStore);
GET_MODEL_TREEVIEW(commands, commandsTreeView, Gtk::ListStore);
GET_MODEL_TREEVIEW(sats, satsTreeview, Gtk::TreeStore);
*/

Glib::RefPtr<Gtk::ListStore> MainWindowRenderer::get_model_treeview_alias()
{
    Glib::RefPtr<Gtk::ListStore> model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(this->aliasTreeview->get_model());

    if(model)
        return model;

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get the alias model", __FILE__, __LINE__);

    return model;
}

Glib::RefPtr<Gtk::ListStore> MainWindowRenderer::get_model_treeview_commands()
{
    Glib::RefPtr<Gtk::ListStore> model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(this->commandsTreeView->get_model());

    if(model)
        return model;

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get the commands model", __FILE__, __LINE__);

    return model;
}

Glib::RefPtr<Gtk::TreeStore> MainWindowRenderer::get_model_treeview_sats()
{
    Glib::RefPtr<Gtk::TreeStore> model = Glib::RefPtr<Gtk::TreeStore>::cast_dynamic(this->satsTreeview->get_model());

    if(model)
        return model;

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get the satellites model", __FILE__, __LINE__);

    return model;
}

/*
#define GET_ROW_TREEVIEW(NAME, CLASS) \
CLASS::ROW MainWindowRenderer::get_row_treeview_NAME(Gtk::TreePath path) \
{ \
    if(path) { \
        CLASS::iterator it = this->get_model_treeview_NAME()->get_iter(path); \
        if(it) \
            return *it; \
    } \
\
    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get row in treeview", __FILE__, __LINE__);\
}

GET_ROW_TREEVIEW(alias,Gtk::ListStore);
GET_ROW_TREEVIEW(commands, Gtk::ListStore);
GET_ROW_TREEVIEW(sats, Gtk::TreeStore);
*/

Gtk::ListStore::Row MainWindowRenderer::get_row_treeview_alias(Gtk::TreePath path)
{
    if(path) {
        Gtk::ListStore::iterator it = this->get_model_treeview_alias()->get_iter(path);
        if(it)
            return *it;
    }

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get row of alias treeview", __FILE__, __LINE__);
}


Gtk::TreeStore::Row MainWindowRenderer::get_row_treeview_sats(Gtk::TreePath path)
{
    if(path) {
        Gtk::ListStore::iterator it = this->get_model_treeview_sats()->get_iter(path);
        if(it)
            return *it;
    }

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get row of alias treeview", __FILE__, __LINE__);
}

Gtk::TreeStore::Row MainWindowRenderer::get_row_treeview_commands(Gtk::TreePath path)
{
    if(path) {
        Gtk::ListStore::iterator it = this->get_model_treeview_commands()->get_iter(path);
        if(it)
            return *it;
    }

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get row of commands treeview", __FILE__, __LINE__);
}

Glib::ustring MainWindowRenderer::get_config_sat_name() {
    if(this->configSatNameLabel) {
        return this->configSatNameLabel->get_text();
    }

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to load the current sat name", __FILE__, __LINE__);    
}

Glib::ustring MainWindowRenderer::get_config_script_name() {
    if(this->configScriptNameLabel) {
        return this->configScriptNameLabel->get_text();
    }

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to load the current script name", __FILE__, __LINE__);
}
