/*
 * CLASS MAIN_WINDOW_CALLBACK : 
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "main_window_callback.hpp"
#include "log.hpp"

/*
 *  Auxiliary fonctions | It's always good to be 
 *  modular
 */
void openFifoFile(int *fifo_fd);

MainWindowCallback::MainWindowCallback(Manager *man, InOutInterface *inter)
{
    this->man = man;
    this->inter = inter;
    this->main_window_renderer = new MainWindowRenderer(man, inter);
    openFifoFile(&this->fifo_fd);

    this->connect_callbacks();
}

void MainWindowCallback::connect_callbacks()
{
    /* Connect sats treeview callbacks */
    this->main_window_renderer->satsTreeview->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindowCallback::sats_treeview_activated_cb));

    /* Connect timeout callbacks */
    sigc::slot<bool> my_slot = sigc::mem_fun(this,&MainWindowCallback::update_curr_satellite);
    Glib::signal_timeout().connect(my_slot, UPDATE_RATE);

    /* Connect alias commands callbacks */
    this->main_window_renderer->commandsTreeView->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindowCallback::command_treeview_activated_cb));

    /* Connect alias treeview callbacks */
    ((Gtk::CellRendererText*)(this->main_window_renderer->aliasAliasColumnRenderer[0]))->signal_edited().connect(sigc::mem_fun(*this, &MainWindowCallback::cellrender_column_alias_edited_cb));
    ((Gtk::CellRendererText*)(this->main_window_renderer->commandsAliasColumnRenderer[0]))->signal_edited().connect(sigc::mem_fun(*this, &MainWindowCallback::cellrender_column_command_edited_cb));
}

void MainWindowCallback::sats_treeview_activated_cb(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
    Glib::ustring scriptName;
    Glib::ustring satName;
    Gtk::TreeStore::Row currRow = (this->main_window_renderer->get_row_treeview_sats(path));

    Gtk::TreeStore::iterator par = currRow.parent();
    if(par) {
        satName = (*par).get_value(modelSatsColumns.col_sat_name);
        scriptName = currRow.get_value(modelSatsColumns.col_script_name);

        this->main_window_renderer->render_new_alias(this->man->GetModelAliasList(satName, scriptName));

        this->main_window_renderer->render_new_text_editor(this->man->GetTextBuffer(satName, scriptName));

        this->main_window_renderer->render_new_sat_name(satName);
        this->main_window_renderer->render_new_scritp_name(scriptName);
    }
}

void MainWindowCallback::cellrender_column_alias_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text) 
{
    Gtk::TreePath treePath(path);
    Gtk::ListStore::iterator it;

    if(treePath) {
        it = this->main_window_renderer->get_model_treeview_alias()->get_iter(treePath);

        if(it)
            this->main_window_renderer->render_alias_column_refresh(it, new_text, this->modelAliasColumns.col_alias);
    }
}

void MainWindowCallback::cellrender_column_command_edited_cb(const Glib::ustring& path, const Glib::ustring& new_text)
{
    Glib::ustring new_alias("new_alias");
    Glib::ustring new_command("new_command");
    Gtk::TreePath treePath(path);

    Glib::ustring old_text;

    if(treePath) {
        Gtk::TreeModel::Row row = this->main_window_renderer->get_row_treeview_alias(treePath);

        if(row) {
            old_text = row.get_value(this->modelAliasColumns.col_command);

            // If we are adding a new command
            // than add new row
            if(!old_text.compare(new_command) && new_text.compare(new_command)) {
                this->main_window_renderer->render_new_alias_row(new_alias, new_command);
            }

            // Updates the changed column
            this->main_window_renderer->render_alias_column_refresh(*row, new_text, this->modelAliasColumns.col_alias);
        }
    }
}

void MainWindowCallback::command_treeview_activated_cb(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
    Glib::ustring satName = this->main_window_renderer->get_config_sat_name();
    Glib::ustring scriptName = this->main_window_renderer->get_config_script_name();

    if(this->man->existsSat(&satName)) {
        Glib::ustring command = this->main_window_renderer->get_row_treeview_commands(path)->get_value(this->modelCommandsColumns.col_command_name);
    
        this->main_window_renderer->render_text_editor_insert_command(command);
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

bool MainWindowCallback::update_curr_satellite()
{
    char *satName, *satEl, *satAz;

    satName = read_fifo_format(this->fifo_fd);
    if(satName == NULL) satName = (char*)"Not Found";
    Glib::ustring satNameStr = satName;
    this->main_window_renderer->render_curr_sat_name_refresh(satNameStr);
    
    satEl = read_fifo_format(this->fifo_fd);
    if(satEl == NULL) satEl = (char*)"Not Set";
    Glib::ustring satElStr = satEl;
    this->main_window_renderer->render_curr_sat_el_refresh(satElStr);

    satAz = read_fifo_format(this->fifo_fd);
    if(satAz == NULL) satAz = (char*)"Not Set";
    Glib::ustring satAzStr = satAz;
    this->main_window_renderer->render_curr_sat_az_refresh(satAzStr);

    return true;
}

/* 
 * TODO:
 * IMPROVE IMPLEMENTATION
 */
void openFifoFile(int *fifo_fd)
{
    *fifo_fd = open(FIFO_FILE, O_RDWR | O_ASYNC | O_NONBLOCK);
    if(*fifo_fd == -1)
        Log::LogWarn(LEVEL_LOG_ERROR, "Unable to load Gpredict data, the program will be closed !", __FILE__, __LINE__);
}
