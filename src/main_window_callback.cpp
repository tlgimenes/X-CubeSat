/*
 * CLASS MAIN_WINDOW_CALLBACK : 
 */
/* X-CubeSat Controler: Real-time communication with satellite program

 Copyright (C)  2014 - Tiago Lobato Gimenes

 Authors: Tiago Lobato Gimenes <tlgimenes@gmail.com>

 Comments, questions and bugreports should be submitted via
 https://github.com/tlgimenes/X-CubeSat
 More details can be found at the project home page:

 https://github.com/tlgimenes/X-CubeSat

 This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

 This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
along with this program; if not, visit http://www.fsf.org/
*/

#include <gdk/gdkkeysyms.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "main_window_callback.hpp"
#include "defs.hpp"
#include "log.hpp"

/*  --------------------------------------------------------  */
MainWindowCallback::MainWindowCallback(Manager *man, Terminal *term)
{
    this->man = man;
    this->term = term;
    this->main_window_renderer = new MainWindowRenderer(man, term);

    this->connect_callbacks();

    this->isRunning = false;

    this->fifo = new std::ifstream(FIFO_FILE);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
Gtk::Window *MainWindowCallback::get_main_window()
{
    if(this->main_window_renderer->mainWindow)
        return this->main_window_renderer->get_main_window();

    return NULL;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void MainWindowCallback::connect_callbacks()
{
    /* Connect hot keys callbacks */
    this->main_window_renderer->mainWindow->signal_key_press_event().connect(sigc::mem_fun(*this, &MainWindowCallback::on_key_press_event));
    this->main_window_renderer->mainWindow->signal_delete_event().connect(sigc::mem_fun(*this, &MainWindowCallback::quit_all_cb));

    /* Connect sats treeview callbacks */
    this->main_window_renderer->satsTreeview->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindowCallback::sats_treeview_activated_cb));
    ((Gtk::CellRendererText*)(this->main_window_renderer->satsTreeview->get_column(this->modelSatsColumns.col_script_name_int)->get_cells()[0]))->signal_edited().connect(sigc::mem_fun(*this, &MainWindowCallback::cellrender_column_scripts_name_edited_cb));
 
    /* Connect alias treeview callbacks */
    ((Gtk::CellRendererText*)(this->main_window_renderer->aliasAliasColumnRenderer[0]))->signal_edited().connect(sigc::mem_fun(*this, &MainWindowCallback::cellrender_column_alias_edited_cb));
    ((Gtk::CellRendererText*)(this->main_window_renderer->commandsAliasColumnRenderer[0]))->signal_edited().connect(sigc::mem_fun(*this, &MainWindowCallback::cellrender_column_command_edited_cb));

    /* Connect timeout callbacks */
    sigc::slot<bool> my_slot = sigc::mem_fun(this,&MainWindowCallback::timeout_cb);
    Glib::signal_timeout().connect(my_slot, UPDATE_RATE);

    /* Connect commands callbacks */
    this->main_window_renderer->commandsTreeView->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindowCallback::command_treeview_activated_cb));

    /* Connect In/Out interface callbacks */
    this->main_window_renderer->deviceName->signal_activate().connect(sigc::mem_fun(*this, &MainWindowCallback::device_name_entry_activated_cb));
    this->main_window_renderer->deviceSpeedComboBox->signal_changed().connect(sigc::mem_fun(*this, &MainWindowCallback::device_speed_combobox_changed_cb));

    /* Connect the main menu */
    Gtk::ImageMenuItem * imitem = 0;
    Menu * menu = new Menu(this->main_window_renderer, this->man);
    this->main_window_renderer->mainBuilder->get_widget("saveFile", imitem);
    if(imitem != 0)
        imitem->signal_activate().connect(sigc::mem_fun(menu, &Menu::save_activate_cb));
    this->main_window_renderer->mainBuilder->get_widget("saveFileAs", imitem);
    if(imitem != 0)
        imitem->signal_activate().connect(sigc::mem_fun(menu, &Menu::saveAs_activate_cb));
    this->main_window_renderer->mainBuilder->get_widget("openFile", imitem);
    if(imitem != 0)
        imitem->signal_activate().connect(sigc::mem_fun(menu, &Menu::open_activate_cb));
    this->main_window_renderer->mainBuilder->get_widget("quit", imitem);
    if(imitem != 0)
        imitem->signal_activate().connect(sigc::mem_fun(*this, &MainWindowCallback::quit_cb));
    this->main_window_renderer->mainBuilder->get_widget("about", imitem);
    if(imitem != 0)
        imitem->signal_activate().connect(sigc::mem_fun(menu, &Menu::about_activate_cb));

    /* Connect the scripts frame */
    this->main_window_renderer->newScriptButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindowCallback::new_script_button_clicked_cb));
    this->main_window_renderer->upButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindowCallback::up_button_clicked_cb));
    this->main_window_renderer->downButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindowCallback::down_button_clicked_cb));

    /* Connect Terminal frame */
    this->main_window_renderer->modemConfig->signal_released().connect(sigc::mem_fun(*this, &MainWindowCallback::on_modem_mode_change_cb));
    this->main_window_renderer->modemFree  ->signal_released().connect(sigc::mem_fun(*this, &MainWindowCallback::on_modem_mode_change_cb));
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void MainWindowCallback::sats_treeview_activated_cb(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
    Glib::ustring scriptName;
    Glib::ustring satName;
    Gtk::TreeStore::Row currRow = (this->main_window_renderer->get_row_treeview_sats(path));

    Gtk::TreeStore::iterator par = currRow.parent();
    if(par) {
        satName = (*par).get_value(modelSatsColumns.col_sat_name);
        scriptName = currRow.get_value(modelSatsColumns.col_script_name);

        if(this->man->exists_script(&satName, &scriptName)) {
            this->main_window_renderer->render_new_alias(this->man->get_model_alias_list(satName, scriptName));

            this->main_window_renderer->render_new_text_editor(this->man->get_text_buffer(satName, scriptName));

            this->main_window_renderer->render_new_sat_name(satName);
            this->main_window_renderer->render_new_scritp_name(scriptName);

            this->main_window_renderer->render_scripts_priority_queue(this->man->get_scripts_priority_queue(&satName));
        }
        else
            Log::LogWarn(LEVEL_LOG_WARNING, "Unable to find this script in database, have you already saved it ?", __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void MainWindowCallback::cellrender_column_alias_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text) 
{
    Glib::ustring satName = this->main_window_renderer->get_config_sat_name();
    Glib::ustring scriptName = this->main_window_renderer->get_config_script_name();

    this->man->replace_alias_column_alias(satName, scriptName, path, new_text);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void MainWindowCallback::cellrender_column_command_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text)
{
    Glib::ustring satName = this->main_window_renderer->get_config_sat_name();
    Glib::ustring scriptName = this->main_window_renderer->get_config_script_name();

    this->man->replace_alias_column_command(satName, scriptName, path, new_text);
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void MainWindowCallback::command_treeview_activated_cb(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
    Glib::ustring satName = this->main_window_renderer->get_config_sat_name();
    Glib::ustring scriptName = this->main_window_renderer->get_config_script_name();

    if(this->man->exists_sat(&satName)) {
        Glib::ustring command = this->main_window_renderer->get_row_treeview_commands(path)->get_value(this->modelCommandsColumns.col_command_name);
    
        this->main_window_renderer->render_text_editor_insert_command(command);
    }
    else {
        Log::LogWarn(LEVEL_LOG_INFO, "open a script associated with a satellite first", __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void MainWindowCallback::device_name_entry_activated_cb()
{
    try {
        Glib::ustring iconName;

        Glib::ustring deviceName = this->main_window_renderer->get_device_name();
        int speed = std::stoi(this->main_window_renderer->get_row_combobox_device_active().get_value(this->modelPortSpeedComboBox.speedName).c_str());
        
        if(this->term->get_interface()->open(deviceName, speed)) {
            iconName = "gtk-yes";
        }
        else {
            iconName = "gtk-no";
        }
        this->main_window_renderer->deviceNameStatus->clear();
        this->main_window_renderer->deviceNameStatus->set_from_icon_name(iconName, Gtk::ICON_SIZE_BUTTON);

        if(this->term->get_interface()->is_configured())
            iconName = "gtk-yes";
        else
            iconName = "gtk-no";
        this->main_window_renderer->deviceSpeedStatus->clear();
        this->main_window_renderer->deviceSpeedStatus->set_from_icon_name(iconName, Gtk::ICON_SIZE_BUTTON);
    }
    catch(std::invalid_argument &exp) {
        Log::LogWarn(LEVEL_LOG_INFO, "Choose a speed", __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void MainWindowCallback::device_speed_combobox_changed_cb()
{
    try {
        Glib::ustring iconName;
        int speed = std::stoi(this->main_window_renderer->get_row_combobox_device_active().get_value(this->modelPortSpeedComboBox.speedName).c_str());

        if(this->term->get_interface()->set_device_speed(speed)) {
            iconName = "gtk-yes";
        }
        else {
            iconName = "gtk-no";
        }
        this->main_window_renderer->deviceSpeedStatus->clear();
        this->main_window_renderer->deviceSpeedStatus->set_from_icon_name(iconName, Gtk::ICON_SIZE_BUTTON);
    } 
    catch (std::invalid_argument& arg) {
        Log::LogWarn(LEVEL_LOG_INFO, "Choose a speed", __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void MainWindowCallback::new_script_button_clicked_cb()
{
    Glib::ustring *scriptName = new Glib::ustring(DEFAULT_SCRIPT_NAME);
    Glib::ustring *scriptData = new Glib::ustring(DEFAULT_CODE_NAME);
    Glib::ustring *alias      = new Glib::ustring(DEFAULT_ALIAS);
    Interpreter   *interpreter= new XCubeSatInterpreter(this->main_window_renderer->term); 

    Glib::ustring satName;

    Gtk::TreeStore::iterator it = this->main_window_renderer->satsTreeview->get_selection()->get_selected();

    Gtk::TreeStore::iterator par = it->parent();
    if(par)
        it = par;
    satName = it->get_value(this->modelSatsColumns.col_sat_name);
    scriptName->append(satName.c_str());

    this->man->add_script(&satName, scriptName, scriptData, alias, interpreter);

    this->main_window_renderer->render_sats_list_refresh(this->man);
    this->main_window_renderer->render_scripts_priority_queue(this->man->get_scripts_priority_queue(&satName));

}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void MainWindowCallback::up_button_clicked_cb()
{
    Glib::ustring satName = this->main_window_renderer->get_config_sat_name();
    int index;

    if(this->man->exists_sat(&satName)) {
        Gtk::ListStore::iterator selectedRow = this->main_window_renderer->scriptsExeQueueTreeview->get_selection()->get_selected();

        Gtk::TreePath path(selectedRow);
        if(path.size() > 0) 
            index = std::stoi(path.to_string());
        else
            index = -1;

        if(index > 0) {
            Gtk::ListStore::iterator upperRow = selectedRow--;

            Glib::RefPtr<Gtk::ListStore> model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(this->main_window_renderer->scriptsExeQueueTreeview->get_model());

            model->iter_swap(upperRow, selectedRow);

            this->man->increase_priority(index, satName);
        }
    }
    else {
        Log::LogWarn(LEVEL_LOG_WARNING, "Choose a satellite before doing this action", __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void MainWindowCallback::down_button_clicked_cb()
{
    Glib::ustring satName = this->main_window_renderer->get_config_sat_name();
    int index;

    if(this->man->exists_sat(&satName)) {
        Gtk::ListStore::iterator selectedRow = this->main_window_renderer->scriptsExeQueueTreeview->get_selection()->get_selected();

        Gtk::TreePath path(selectedRow);
        if(path.size() > 0)
            index = std::stoi(path.to_string());
        else
            index = -1;

        if(index > -1) {
            Gtk::ListStore::iterator lowerRow = selectedRow++;

            Glib::RefPtr<Gtk::ListStore> model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(this->main_window_renderer->scriptsExeQueueTreeview->get_model());

            model->iter_swap(lowerRow, selectedRow);

            this->man->decrease_priority(index, satName);
        }
    }
    else {
        Log::LogWarn(LEVEL_LOG_WARNING, "Choose a satellite before doing this action", __FILE__, __LINE__);
    }  
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/*
 * TODO:
 * Improove newScriptName verification for the user does
 * not give an invalid name to a script
 */
void MainWindowCallback::cellrender_column_scripts_name_edited_cb(const Glib::ustring &path, const Glib::ustring &new_text)
{
    Glib::ustring *newScriptName = new Glib::ustring();
    Glib::ustring *oldScriptName = new Glib::ustring();
    Glib::ustring *satName = new Glib::ustring();

    Gtk::TreePath treepath(path);
    Gtk::TreeStore::Row currRow = (this->main_window_renderer->get_row_treeview_sats(treepath));

    Gtk::TreeStore::iterator par = currRow.parent();
    if(par) {
        *satName = (*par).get_value(modelSatsColumns.col_sat_name);
        *oldScriptName = currRow.get_value(modelSatsColumns.col_script_name);
        *newScriptName = new_text;

        /* Renames the script in the Config frame */
        if(main_window_renderer->configSatNameLabel != NULL && \
                !satName->compare(main_window_renderer->configSatNameLabel->get_text()) && \
                main_window_renderer->configSatNameLabel != NULL && \
                !oldScriptName->compare(main_window_renderer->configScriptNameLabel->get_text()))
            this->main_window_renderer->configScriptNameLabel->set_text(*newScriptName);

        this->man->rename_script(satName, oldScriptName, newScriptName);

        currRow.set_value(modelSatsColumns.col_script_name, new_text);
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */  
/* Reads the FIFO file. The format is :
 * SatName'\n'
 * Elevation'\n'
 * Azimuth'\n'
 */
//fifo_file_model *read_fifo_format(const char *fifoName)
fifo_file_model *read_fifo_format(std::ifstream **fifo)
{
    char line[MAX_M_SIZE];
    fifo_file_model * fifofm = new fifo_file_model[1];
    static bool lock = true;

    /* Opens fifo file */
    //std::ifstream fifo(fifoName);
    if(!(*fifo)->is_open() && !lock) {
        (*fifo)->close();
        delete(*fifo);
        *fifo = new std::ifstream(FIFO_FILE);
        lock = !lock;
    }

    if(**fifo) {
        (*fifo)->getline(line, MAX_M_SIZE);
        fifofm->satName = new std::string(line);

        (*fifo)->getline(line, MAX_M_SIZE);
        fifofm->el = new std::string(line);

        (*fifo)->getline(line, MAX_M_SIZE);
        fifofm->az = new std::string(line);
    }
    else {
        fifofm->satName = NULL;
        fifofm->el      = NULL;
        fifofm->az      = NULL;
    }

    //fifo.close();
    return fifofm;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool MainWindowCallback::timeout_cb()
{
    /* Update Current Satellite */
    this->update_curr_satellite();

    /* Update terminal */
    this->main_window_renderer->term->update();

    return true;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void MainWindowCallback::update_curr_satellite()
{
 //   fifo_file_model *m = read_fifo_format(FIFO_FILE);
    fifo_file_model *m = read_fifo_format(&this->fifo);

    /* Renders the new info in CurrSat frame */
    if(m->satName == NULL) m->satName = new std::string("Not Found");
    this->main_window_renderer->render_curr_sat_name_refresh((*m->satName));

    if(m->el == NULL) m->el = new std::string("Not Set");
    this->main_window_renderer->render_curr_sat_el_refresh((*m->el));

    if(m->az == NULL) m->az = new std::string("Not Set");
    this->main_window_renderer->render_curr_sat_az_refresh((*m->az));

    /* Runs the script to be runned */
    if(!isRunning && m->el->compare("Not Set")) {
        try {
            if(std::stof(m->el->c_str()) >= 0.0f) {
                this->isRunning = true;
                this->main_window_renderer->render_curr_status_refresh("running script");
                this->man->run_next_script((*m->satName));
                this->isRunning = false;
                this->main_window_renderer->render_curr_status_refresh("Idle");
            }
        }
        catch (std::exception &e) {
            /* ERROR in FIFO FILE ! What TODO next ? */
        }
    }
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void MainWindowCallback::on_modem_mode_change_cb()
{
    this->main_window_renderer->term->change_mode();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
#define SAVE_SESSION() \
    this->man->save(); \
    Log::LogWarn(LEVEL_LOG_INFO, "Session Saved", __FILE__, __LINE__); 
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool MainWindowCallback::on_key_press_event(GdkEventKey *event)
{
    Glib::ustring session = DEFAULT_SESSION_FILE; 

    switch(event->keyval) {
        case GDK_KEY_S:
        case GDK_KEY_s:
            SAVE_SESSION();
            break;
        case GDK_KEY_Escape:
            this->quit_all_cb(NULL);
            break;
    }

    return false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void MainWindowCallback::quit_cb()
{
    Glib::ustring session = DEFAULT_SESSION_FILE; 
    this->man->save();
    Log::LogWarn(LEVEL_LOG_INFO, "Session Saved", __FILE__, __LINE__); 
 
    gtk_main_quit();
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
bool MainWindowCallback::quit_all_cb(GdkEventAny *event)
{
    Glib::RefPtr<Gtk::Builder> bd = Gtk::Builder::create_from_file(DIALOG_WINDOW_GLADE);
    Gtk::MessageDialog *quitDialog;

    // Get the widget
    bd->get_widget(QUIT_MESSAGE_DIALOG, quitDialog);

    quitDialog->set_default_response(Gtk::RESPONSE_YES);
    quitDialog->signal_response().connect(sigc::mem_fun(*this, &MainWindowCallback::on_response_save_message_dialog));
    quitDialog->run();
    quitDialog->hide();

    gtk_main_quit();

    return false;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
void MainWindowCallback::on_response_save_message_dialog(int response_id)
{
    Glib::ustring session = DEFAULT_SESSION_FILE; 

    if(response_id == Gtk::RESPONSE_YES) {
        this->man->save();
        Log::LogWarn(LEVEL_LOG_INFO, "Session saved", __FILE__, __LINE__);
    }
}
/*  --------------------------------------------------------  */
