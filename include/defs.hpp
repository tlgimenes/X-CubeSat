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

#ifndef DEFS_HPP
#define DEFS_HPP

#define DEBUG 1
#define DEFAULT_OUTPUT              "/dev/ttyS0"
#define DEFAULT_SPEED               9600
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

/* ---------------------------------------------------- */
/* Menu */
/* ---------------------------------------------------- */
#define SAVEAS_FILE_WIDGET "saveFileAs"
#define OPEN_FILE_WIDGET "openFile"
#define QUIT_WIDGET "quit"
#define ABOUT_FILE_WIDGET "about"
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
/* Sats List */
/* ---------------------------------------------------- */
#define SATS_TREEVIEW_WIDGET "satsTreeview"
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
/* Curr sat */
/* ---------------------------------------------------- */
#define CURR_SAT_LABEL_WIDGET "sat"
#define STATUS_LABEL_WIDGET "stat"
#define ELEVATION_LABEL_WIDGET "elevation"
#define AZIMUTH_LABEL_WIDGET "azimuth"
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
/* Text Editor */
/* ---------------------------------------------------- */
#define TEXT_VIEW_WIDGET "commandsFileText"
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
/* Alias */
/* ---------------------------------------------------- */
#define ALIAS_TREEVIEW_WIDGET "aliasTreeview"
/* ---------------------------------------------------- */


/* ---------------------------------------------------- */
/* Config */
/* ---------------------------------------------------- */
#define CONFIG_SAT_NAME_WIDGET "satNameLabel"
#define CONFIG_SCRIPT_NAME_WIDGET "scriptNameLabel"
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
/* Port Config */
/* ---------------------------------------------------- */
#define PORT_NAME_ENTRY_WIDGET "portName"
#define PORT_SPEED_COMBOBOX_WIDGET "serialPortComboBox"
#define PORT_NAME_STATUS_WIDGET "serialPortNameStatus"
#define UPS_SPEED_STATUS_WIDGET "upsStatus"
#define DEFAULT_BAUD_RATE 9600
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
/* Commands */
/* ---------------------------------------------------- */
#define COMMANDS_TREEVIEW_WIDGET "commandsTreeview"
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
/* Scripts */
/* ---------------------------------------------------- */
#define SCRIPTS_EXE_QUEUE_TREEVIEW "scriptsExeQueueTreeview"
#define INCREASE_PRIORITY_BUTTON "increasePriorityButton"
#define DECREASE_PRIORITY_BUTTON "decreasePriorityButton"
#define ADD_NEW_SCRIPT_BUTTON "addNewScriptButton"
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
#define DEFAULT_SCRIPT_NAME "new_script_"
#define DEFAULT_CODE_NAME "#Write your code here\n"
#define DEFAULT_ALIAS "OK\ninsert_ok\nnew_alias\nnew_command\n"
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
#define QUIT_MESSAGE_DIALOG "quitDialog"
/* ---------------------------------------------------- */


/* ---------------------------------------------------- */
#define LOG_FILE "XCubeSat_Controler.txt"

#define DIALOG_WINDOW_GLADE "windows/dialogWindow.glade"
#define WARN_DIALOG_WIDGET  "warnDialog"
#define ERROR_DIALOG_WIDGET "errorDialog"
#define INFO_DIALOG_WIDGET  "infoDialog"
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
#define MAX_COMMENTS_SIZE 999
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
/* DATABASE DEFINITIONS */
/* ---------------------------------------------------- */
#define DATABASE_NAME "dataBase/XCubeSat_Controler.db"
#define DATABASE_CONFIG "dataBase/tables.sql"
#define SCI_DATA_TABLE "XCubeSat_Controler_sciData"
#define WOD_DATA_TABLE "XCubeSat_Controler_wodData" 
#define SCRIPTS_DATA_TABLE "XCubeSat_Controler_scripts"
#define SESSION_DATA_TABLE "XCubeSat_Controler_session"
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
/* Terminal Definitions */
#define TERMINAL_TEXTVIEW "terminalTextView"
#define MODEM_CONFIG_RADIO_BUTTON "modemConfigModeButton"
#define MODEM_FREE_RADIO_BUTTON "modemFreeModeButton"
#define NON_EDITABLE_TAG_NAME "notEditableTerminalTag"
#define MAX_BUFFER_SIZE 20
#define MAX_MESSAGE_SIZE 999
/* ---------------------------------------------------- */

#endif
