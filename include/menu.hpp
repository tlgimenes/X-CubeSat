/*
 * ABOUT_HPP: Show the about dialog
 *
 */

#ifndef ABOUT_HPP
#define ABOUT_HPP

#define OPEN_GLADE_FILE "windows/openWindow.glade"
#define SAVEAS_GLADE_FILE "windows/saveAsWindow.glade"
#define ABOUT_GLADE_FILE "windows/aboutWindow.glade"

#define OPEN_WIDGET "openWindow"
#define SAVEAS_WIDGET "saveAsWindow"
#define ABOUT_WIDGET "aboutWindow"

void open_activate_cb();
void saveAs_activate_cb();
void about_activate_cb();

#endif
