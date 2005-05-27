/* Darimasen - directory.h - Copyright (C) 2005 Sudrien, GPL */

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <gtkmm.h>
#include <vector>
#include "icon.h"
#include "tabs.h"
#include <iostream>

class directory : public Gtk::EventBox {
  public:
    directory(class tabs&, gint);
    ~directory();

  private:

  class tabs * parent;
  gint position;

  class icon icon_list;
  Gtk::ScrolledWindow scroller;

  Gtk::TreeView tree_view;
  Glib::RefPtr< Gtk::ListStore > tree_model;

  
  std::vector< Glib::RefPtr<const Gnome::Vfs::FileInfo > > file_list;


  bool add_file(Glib::ustring,Glib::RefPtr<const Gnome::Vfs::FileInfo>, bool, bool);
  int sort_list_func(const Gtk::TreeModel::iterator&, const Gtk::TreeModel::iterator&);

//  void render();
  };


#endif
