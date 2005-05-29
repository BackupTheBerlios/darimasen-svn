/* Darimasen - menu.cpp - Copyright (C) 2005 Sudrien, GPL */

#include "menu.h"

darimasen_menu::darimasen_menu(){
Gtk::MenuBar::MenuBar();
items().push_back(Gtk::Menu_Helpers::MenuElem(
    "\342\226\274"));
  show();
  }
