/*
 * CLASS SAT_MANAGER : Each manager has one satellite
 * where it cans send and receive data.
 *
 */

#ifndef SAT_MANAGER_HPP
#define SAT_MANAGER_HPP

#include <unordered_map>
#include <queue>
#include <string>

#include "satellite.hpp"
#include "script.hpp"
#include "interpreter.hpp"
#include "models.hpp"

#define MAX_SAT_NAME_SIZE 400

class SatManager
{
    private:
        Satellite * sat;

        std::unordered_map<std::string, Script*> scripts;

        std::vector<Script*>         scriptsQueue;
        Glib::RefPtr<Gtk::ListStore> scriptsPriorityQueue;
        ModelScriptsPriorityQueue    modelScriptsPriorityQueue;

        Gtk::TreeModel::RowReference *row;
        ModelSatsColumns             *modelSatsColumns;

    public:
       // SatManager(Satellite *sat);
        SatManager(Satellite *sat, Gtk::TreeModel::RowReference *row, ModelSatsColumns *model);

        Glib::RefPtr<Gtk::TextBuffer> *get_text_buffer(Glib::ustring script);
        Glib::RefPtr<Gtk::ListStore>  *get_model_alias_list(Glib::ustring script);
        Gtk::TreeModel::RowReference  *get_row_reference();
        std::stringstream             *get_first_alias();
        std::stringstream             *get_alias(Glib::ustring script);
        Glib::RefPtr<Gtk::ListStore>  *get_scripts_priority_queue();

        void add_script(Glib::ustring *name, Glib::ustring *script, Glib::ustring *aliasList, Interpreter *inter);
        void enqueue_script(Glib::ustring scriptName);
        void run_script(Glib::ustring scriptName);
        void run_next_script();
        void rename_script(Glib::ustring *oldName, Glib::ustring *newName);

        void increase_priority(int index);
        void decrease_priority(int index);

        bool exists_script(Glib::ustring name);

        std::stringstream *get_save_str(Glib::ustring sessionFile);
};

#endif
