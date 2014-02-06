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
#include "sats.hpp"

class SatManager
{
    private:
        Satellite * sat;

        std::unordered_map<std::string, Script*> scripts;
        std::queue<Script*> scriptsQueue;

        ModelSatsColumns *modelSatsColumns;
        Gtk::TreeModel::RowReference *row;

    public:
       // SatManager(Satellite *sat);
        SatManager(Satellite *sat, Gtk::TreeModel::RowReference *row, ModelSatsColumns *model);

        void AddScript(Glib::ustring *name, Glib::ustring *script, Glib::ustring *aliasList, Interpreter *inter);
        void RunScript(Glib::ustring scriptName);

        void EnqueueScript(Glib::ustring scriptName);
        void RunNextScript();

        Gtk::TreeModel::RowReference *GetRowReference();
        Glib::RefPtr<Gtk::TextBuffer> *GetTextBufffer(Glib::ustring script);
        Glib::RefPtr<Gtk::ListStore> *GetModelAliasList(Glib::ustring script);
        std::stringstream *GetFirstAlias();
        std::stringstream *GetAlias(Glib::ustring script);
};

#endif
