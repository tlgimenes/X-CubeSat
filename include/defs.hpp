#ifndef DEFS_HPP
#define DEFS_HPP

#define DEBUG 1
#define DEFAULT_OUTPUT              "/dev/tty1"
#define DEFAULT_SESSION_FILE        "defaults/XCubeSat_Controler.session"
#define DEFAULT_GPREDICT_SATS_FILE  "/tmp/GpredictSATS"

/* ---------------------------------------------------- */

#define MAIN_WINDOW_GLADE "windows/mainWindow.glade"

#define MAIN_WINDOW_WIDGET "mainWindow"

#define FIFO_FILE "/tmp/GpredictFIFO"
#define SATS_FILE "/tmp/GpredictSATS"

#define MAX_SAT_NAME 400
#define MAX_M_SIZE   430

#define UPDATE_RATE  1000

/*
 * Menu
 */
#define SAVEAS_FILE_WIDGET "saveFileAs"
#define OPEN_FILE_WIDGET "openFile"
#define QUIT_WIDGET "quit"
#define ABOUT_FILE_WIDGET "about"

/*
 * Sats List
 */
#define SATS_TREEVIEW_WIDGET "satsTreeview"

/*
 * Curr sat
 */
#define CURR_SAT_LABEL_WIDGET "sat"
#define STATUS_LABEL_WIDGET "status"
#define ELEVATION_LABEL_WIDGET "elevation"
#define AZIMUTH_LABEL_WIDGET "azimuth"

/*
 * Text Editor
 */
#define TEXT_VIEW_WIDGET "commandsFileText"

/*
 * Alias
 */
#define ALIAS_TREEVIEW_WIDGET "aliasTreeview"


/*
 * Config
 */
#define CONFIG_SAT_NAME_WIDGET "satNameLabel"
#define CONFIG_SCRIPT_NAME_WIDGET "scriptNameLabel"

/*
 * Port Config
 */
#define PORT_NAME_ENTRY_WIDGET "portName"
#define PORT_SPEED_COMBOBOX_WIDGET "serialPortComboBox"
#define PORT_NAME_STATUS_WIDGET "serialPortNameStatus"
#define UPS_SPEED_STATUS_WIDGET "upsStatus"

/*
 * Commands
 */
#define COMMANDS_TREEVIEW_WIDGET "commandsTreeview"

/*
 * Scripts
 */
#define SCRIPTS_EXE_QUEUE_TREEVIEW "scriptsExeQueueTreeview"
#define INCREASE_PRIORITY_BUTTON "increasePriorityButton"
#define DECREASE_PRIORITY_BUTTON "decreasePriorityButton"
#define ADD_NEW_SCRIPT_BUTTON "addNewScriptButton"



#endif
