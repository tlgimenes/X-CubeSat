/*
 * CLASS MANAGER : This class is the core class of 
 * this program. It gives methods to add some config 
 * files to satellites, enqueue new data to send and 
 * receive data from the satellites.
 *
 */

#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <string>
#include <unordered_map>

#include "sat_manager.hpp"
#include "models.hpp"

class Manager
{
    private:
        std::unordered_map<std::string, SatManager*> satManagers;
        Glib::ustring *tracking;
        float elevation;
        
        Glib::RefPtr<Gtk::TreeStore> satsTreeStore;
        ModelSatsColumns *modelSatsColumns;

    public:
        Manager();

        void add_sat(Glib::ustring *satName);

        void add_script(Glib::ustring *satName, Glib::ustring *scriptName, Glib::ustring *script, Glib::ustring *aliasList, Interpreter *inter);

        void replace_alias_column_alias(Glib::ustring satName, Glib::ustring scriptName, const Glib::ustring& path, const Glib::ustring& newAlias);
        void replace_alias_column_command(Glib::ustring satName, Glib::ustring scriptName, const Glib::ustring& path, const Glib::ustring& newAlias);

        bool exists_sat(Glib::ustring *satName);
        bool exists_script(Glib::ustring *satName, Glib::ustring *scriptName);

        Glib::RefPtr<Gtk::TextBuffer> *get_text_buffer(Glib::ustring satName, Glib::ustring script);
        Glib::RefPtr<Gtk::ListStore>  *get_model_alias_list(Glib::ustring satName, Glib::ustring script);
        Glib::RefPtr<Gtk::TreeStore>  *get_model_sats_store();

        std::stringstream *get_first_alias(Glib::ustring satName);
        std::stringstream *get_alias(Glib::ustring satName, Glib::ustring scriptName);

        Glib::RefPtr<Gtk::ListStore> *get_scripts_priority_queue(Glib::ustring *satName);

        void increase_priority(int index, Glib::ustring satName);
        void decrease_priority(int index, Glib::ustring satName);

        void save(Glib::ustring sessionFile);
        void rename_script(Glib::ustring *satName, Glib::ustring *oldScriptName, Glib::ustring *newScriptName);

        void run_next_script(Glib::ustring satName);
};

#endif
