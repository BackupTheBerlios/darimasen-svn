/* Darimasen - menu.cpp - Copyright (C) 2005 Sudrien, GPL */

#include "menu.h"

darimasen_menu::darimasen_menu(class darimasen_window& ancestor){
  parent = &ancestor;


  

  
  Gtk::MenuBar * compact = Gtk::manage(new Gtk::MenuBar);
  compact->items().push_back(Gtk::Menu_Helpers::MenuElem("\342\226\274"));


  Glib::ustring path = parent->tabber->get_history(parent->tabber->get_current_page());
  Glib::ustring shortened = path;
  

  if ( path.find( Glib::get_home_dir()) != Glib::ustring::npos ) 
    shortened = "~" + path.substr(Glib::get_home_dir().length());
   std::cout << shortened << "\n";

  compact->items().push_back(Gtk::Menu_Helpers::MenuElem(shortened));

    



  compact->show();

  //Glib::RefPtr<Gtk::RcStyle> compact_style = compact->get_modifier_style();
  Glib::RefPtr<Gtk::Style> compact_style = compact->get_style();
  compact_style->set_xthickness(0);
  compact_style->set_ythickness(0);
  compact->set_style(compact_style);


  compact->show();
  Gtk::ToolItem * compact_container = Gtk::manage(new Gtk::ToolItem);
  compact_container->add(*compact);
  compact_container->show();
  append(*compact_container);
  show();
  }
