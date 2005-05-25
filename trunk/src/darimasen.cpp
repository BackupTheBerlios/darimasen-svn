/* Darimasen - darimasen.cpp - Copyright (C) 2005 Sudrien, GPL */

#include "darimasen.h"

darimasen_window::darimasen_window(std::vector<Glib::ustring> paths){
  tabs * tabber = Gtk::manage(new class tabs(paths));
  add(*tabber);
  show();
  }

darimasen_window::~darimasen_window(){

  }

