/* Darimasen - iconmodes.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef ICONMODES_H
#define ICONMODES_H

/**********************/

#include <gtkmm/table.h>
#include "main.h"
#include <libgnomevfsmm.h>

/**********************/

class DaIconModes : public Gtk::Table {


  class Sidecon : public Gtk::EventBox{
    Glib::ustring filePath;
    Gtk::Menu m_Menu_Popup;   
  public:
    Sidecon(Glib::ustring, const Glib::RefPtr<const Gnome::Vfs::FileInfo>& );
  bool on_eventbox_button_press(GdkEventButton*);
    ~Sidecon();
  };


  Glib::ustring fullPath;
  int filesAtPath;
  int heightAvailable;
  unsigned short iconmode;
  bool showHidden;
  int x_pos, y_pos;

  bool addSidecon(const Glib::ustring&, const Glib::RefPtr<const Gnome::Vfs::FileInfo>&, bool, bool&);
  bool addDetail(const Glib::ustring&, const Glib::RefPtr<const Gnome::Vfs::FileInfo>&, bool, bool&);



public:
  DaIconModes(Glib::ustring, int, unsigned short, int, bool); //path, filesAtPath, mode 
   
  };

/**********************/

#endif //ICONMODES_H
