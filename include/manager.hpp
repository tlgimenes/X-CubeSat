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
#include "sats.hpp"

class Manager
{
    private:
        std::unordered_map<std::string, SatManager*> satManagers;
        Glib::ustring *tracking;
        float elevation;
        
        ModelSatsColumns *modelSatsColumns;
        Glib::RefPtr<Gtk::TreeStore> satsTreeStore;

    public:
        Manager();

        void AddSat(Glib::ustring *satName);

        void AddScript(Glib::ustring *satName, Glib::ustring *scriptName, Glib::ustring *script, Glib::ustring *aliasList, Interpreter *inter);

        bool existsSat(Glib::ustring *satName);

        Glib::RefPtr<Gtk::TextBuffer> *GetTextBuffer(Glib::ustring satName, Glib::ustring script);
        Glib::RefPtr<Gtk::ListStore> *GetModelAliasList(Glib::ustring satName, Glib::ustring script);
        Glib::RefPtr<Gtk::TreeStore> *GetModelSatsStore();

        std::stringstream *GetFirstAlias(Glib::ustring satName);
        std::stringstream *GetAlias(Glib::ustring satName, Glib::ustring scriptName);
};

#endif
