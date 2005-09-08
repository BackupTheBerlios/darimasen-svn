/* Darimasen - darimasen.cpp - Copyright (C) 2005 Sudrien, GPL */

#include "darimasen.h"

darimasen_window::darimasen_window(std::vector<Glib::ustring> paths){
  
  set_title("Darimasen 0.1.x");
  set_default_size(500, 330);

  Gtk::VBox * vert = Gtk::manage(new class Gtk::VBox);
  vert->show();
  add(*vert);



  tabber = Gtk::manage(new class tabs(*this,paths));
  da_menu = Gtk::manage(new class darimasen_menu(*this));

  vert->pack_start(*da_menu, Gtk::PACK_SHRINK, 0);
  vert->pack_start(*tabber);

  Gtk::Statusbar * status = Gtk::manage(new class Gtk::Statusbar);
  status->show();
  status->set_spacing(2);

  Gtk::Label * xed = Gtk::manage(new Gtk::Label("Dropbox Goes Here"));
  xed->show();

  status->pack_start(*xed, Gtk::PACK_SHRINK, 0);

  vert->pack_start(*status, Gtk::PACK_SHRINK, 0);
  //add(*tabber);
  show();
  }

darimasen_window::~darimasen_window(){

  }

