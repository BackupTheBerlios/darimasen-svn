/* Darimasen - sidecon.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef SIDECON_H
#define SIDECON_H

/**********************/

#include <gtkmm/eventbox.h>
#include <gtkmm/table.h>
#include <gtkmm/stock.h>
#include <gtkmm/label.h>
#include <gtkmm/menu.h>
#include <gtkmm/menuitem.h>
#include "main.h"
#include <sys/stat.h>

#ifdef WIN32
#include <windows.h>
#include <direct.h>
#endif

/**********************/

class Sidecon : public Gtk::EventBox{
   Glib::ustring filePath;
   Gtk::Menu m_Menu_Popup;   
public:
  Sidecon(Glib::ustring);
  ~Sidecon();
  bool on_eventbox_button_press(GdkEventButton*);
};

/**********************/

#endif //SIDECON_H
