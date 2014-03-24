#include <gtkmm.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "main_window_renderer.hpp"
#include "log.hpp"

void MainWindowRenderer::init_sats_frame(Manager *man)
{ 
    this->mainBuilder->get_widget(SATS_TREEVIEW_WIDGET, this->satsTreeview);
    Glib::RefPtr<Gtk::ListStore> model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(this->satsTreeview->get_model());

    // For Gtk+3.8 Only
    //this->satsTreeview->set_activate_on_single_click(true); 

    this->satsTreeview->set_model(*man->get_model_sats_store());

    ((Gtk::CellRendererText*)(this->satsTreeview->get_column(this->modelSatsColumns.col_script_name_int)->get_cells()[0]))->property_editable().set_value(true);

}

void MainWindowRenderer::init_curr_sat_frame()
{ 
    this->mainBuilder->get_widget(CURR_SAT_LABEL_WIDGET, this->satName);
    this->mainBuilder->get_widget(STATUS_LABEL_WIDGET, this->status);
    this->mainBuilder->get_widget(ELEVATION_LABEL_WIDGET, this->satEl);
    this->mainBuilder->get_widget(AZIMUTH_LABEL_WIDGET, this->satAz);
}

void MainWindowRenderer::init_alias_frame()
{
    this->mainBuilder->get_widget(ALIAS_TREEVIEW_WIDGET, this->aliasTreeview);

    this->aliasAliasColumnRenderer = this->aliasTreeview->get_column(this->modelAliasColumns.col_alias_int)->get_cells();
    ((Gtk::CellRendererText*)(this->aliasAliasColumnRenderer[0]))->property_editable().set_value(true);

    this->commandsAliasColumnRenderer = this->aliasTreeview->get_column(this->modelAliasColumns.col_command_int)->get_cells();
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

void MainWindowRenderer::init_port_config_frame(InOutInterface *inter)
{
    this->mainBuilder->get_widget(PORT_NAME_ENTRY_WIDGET, this->deviceName);
    this->mainBuilder->get_widget(PORT_NAME_STATUS_WIDGET, this->deviceNameStatus);
    this->mainBuilder->get_widget(PORT_SPEED_COMBOBOX_WIDGET, this->deviceSpeedComboBox);
    this->mainBuilder->get_widget(UPS_SPEED_STATUS_WIDGET, this->deviceSpeedStatus);

    if(inter->is_oppenned()) {
        this->deviceName->set_text(inter->get_device_name());
        this->deviceNameStatus->set_from_icon_name("gtk-yes", Gtk::ICON_SIZE_BUTTON);
    }
    if(inter->is_configured()) {
        this->deviceSpeedStatus->set_from_icon_name("gtk-yes", Gtk::ICON_SIZE_BUTTON);
    }
}

void MainWindowRenderer::init_commands_frame()
{
    this->mainBuilder->get_widget(COMMANDS_TREEVIEW_WIDGET, this->commandsTreeView);
}

void MainWindowRenderer::init_scripts_frame()
{
    this->mainBuilder->get_widget(SCRIPTS_EXE_QUEUE_TREEVIEW, this->scriptsExeQueueTreeview);
    this->mainBuilder->get_widget(INCREASE_PRIORITY_BUTTON, this->upButton);
    this->mainBuilder->get_widget(DECREASE_PRIORITY_BUTTON, this->downButton);
    this->mainBuilder->get_widget(ADD_NEW_SCRIPT_BUTTON, this->newScriptButton);
}

MainWindowRenderer::MainWindowRenderer(Manager *man, InOutInterface *inter)
{
    try {
        this->mainBuilder = Gtk::Builder::create_from_file(MAIN_WINDOW_GLADE);
        this->mainBuilder->get_widget(MAIN_WINDOW_WIDGET, this->mainWindow);

        /* Config */
        init_config_frame();

        /* Sats */
        init_sats_frame(man);

        /* Alias */
        init_alias_frame();
        
        /* Text */
        init_text_editor();

        /* Current Sat */
        init_curr_sat_frame();

        /* Port Config */
        init_port_config_frame(inter);

        /* Commands */
        init_commands_frame();

        /* Scripts frame */
        init_scripts_frame();
    }
    catch(const Glib::FileError& ex) {
        Log::LogWarn(LEVEL_LOG_WARNING, ex.what().c_str(), __FILE__, __LINE__);
    }
};

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

void MainWindowRenderer::render_curr_status_refresh(Glib::ustring status)
{
    if(status.size() > 0) {
        this->status->set_text(status);
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

void MainWindowRenderer::render_sats_list_refresh(Manager *man)
{
    this->satsTreeview->set_model(*man->get_model_sats_store());
}

void MainWindowRenderer::render_scripts_priority_queue(Glib::RefPtr<Gtk::ListStore>* Pqueue)
{
    if(Pqueue != NULL)
        this->scriptsExeQueueTreeview->set_model(*Pqueue);
}

Glib::RefPtr<Gtk::ListStore> MainWindowRenderer::get_model_treeview_alias()
{
    Glib::RefPtr<Gtk::ListStore> model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(this->aliasTreeview->get_model());

    if(model)
        return model;

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get the alias model", __FILE__, __LINE__);

    // Stop annoying compler warns
    return model;
}

Glib::RefPtr<Gtk::ListStore> MainWindowRenderer::get_model_treeview_commands()
{
    Glib::RefPtr<Gtk::ListStore> model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(this->commandsTreeView->get_model());

    if(model)
        return model;

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get the commands model", __FILE__, __LINE__);

    // Stop annoying compler warns
    return model;
}

Glib::RefPtr<Gtk::TreeStore> MainWindowRenderer::get_model_treeview_sats()
{
    Glib::RefPtr<Gtk::TreeStore> model = Glib::RefPtr<Gtk::TreeStore>::cast_dynamic(this->satsTreeview->get_model());

    if(model)
        return model;

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get the satellites model", __FILE__, __LINE__);

    // Stop annoying compler warns
    return model;
}

Gtk::ListStore::Row MainWindowRenderer::get_row_treeview_alias(Gtk::TreePath path)
{
    if(path) {
        Gtk::ListStore::iterator it = this->get_model_treeview_alias()->get_iter(path);
        if(it)
            return *it;
    }

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get row of alias treeview", __FILE__, __LINE__);

    // Stop annoying compiler warns
    Gtk::ListStore::Row row;
    return row;
}


Gtk::TreeStore::Row MainWindowRenderer::get_row_treeview_sats(Gtk::TreePath path)
{
    if(path) {
        Gtk::ListStore::iterator it = this->get_model_treeview_sats()->get_iter(path);
        if(it)
            return *it;
    }

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get row of alias treeview", __FILE__, __LINE__);

    // Stop annoying compiler warns
    Gtk::ListStore::Row row;
    return row;
}

Gtk::TreeStore::Row MainWindowRenderer::get_row_treeview_commands(Gtk::TreePath path)
{
    if(path) {
        Gtk::ListStore::iterator it = this->get_model_treeview_commands()->get_iter(path);
        if(it)
            return *it;
    }

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get row of commands treeview", __FILE__, __LINE__);

    // Stop annoying compiler warns
    Gtk::ListStore::Row row;
    return row;
}


Glib::ustring MainWindowRenderer::get_device_name()
{
    Glib::ustring* deviceName = new Glib::ustring(this->deviceName->get_text());

    if(deviceName->size() <= 0)
        Log::LogWarn(LEVEL_LOG_WARNING, "The port name must be valid", __FILE__, __LINE__);

    return *deviceName;
}
 
Gtk::TreeModel::Row MainWindowRenderer::get_row_combobox_device_active()
{
    Gtk::TreeModel::Row row = (*this->deviceSpeedComboBox->get_active());

    if(!row) {
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to load speed for the port", __FILE__, __LINE__);
    }

    return row;
}

Glib::ustring MainWindowRenderer::get_config_sat_name() {
    if(this->configSatNameLabel) {
        return this->configSatNameLabel->get_text();
    }

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to load the current sat name", __FILE__, __LINE__);    

    // Stop annoying compiler warns
    Glib::ustring garbage("ERROR");
    return garbage;
}

Glib::ustring MainWindowRenderer::get_config_script_name() {
    if(this->configScriptNameLabel) {
        return this->configScriptNameLabel->get_text();
    }

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to load the current script name", __FILE__, __LINE__);

    // Stop annoying compiler warns
    Glib::ustring garbage("ERROR");
    return garbage;
}

Gtk::TextView * MainWindowRenderer::get_text_editor()
{
    if(this->textEditor)
        return this->textEditor;
    
    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get the text editor", __FILE__, __LINE__);

    return NULL;
}

Gtk::Window * MainWindowRenderer::get_main_window()
{
    if(this->mainWindow)
        return this->mainWindow;

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get the main window", __FILE__, __LINE__);

    return NULL;
}

Glib::RefPtr<Gtk::Builder> MainWindowRenderer::get_main_builder()
{
    if(this->mainBuilder)
        return this->mainBuilder;

    Log::LogWarn(LEVEL_LOG_ERROR, "Unable to get window builder", __FILE__, __LINE__);

    return this->mainBuilder;
}

