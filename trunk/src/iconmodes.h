/* Darimasen - iconmodes.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef ICONMODES_H
#define ICONMODES_H

/**********************/

#include <gtkmm/table.h>
#include <gtkmm/messagedialog.h>
#include "main.h"
#include <libgnomevfsmm.h>
#include <gtkmm/entry.h>

#include "iconmodes.h"

/**********************/

class DaIconModes : public Gtk::EventBox {


  class Sidecon : public Gtk::Table{
    Glib::ustring filePath, mimeInfo;
       
    public:
    Sidecon(Glib::ustring, const Glib::RefPtr<const Gnome::Vfs::FileInfo>& );
  
    ~Sidecon();
    //void DaIconModes::Sidecon::RunFile();
    };

  void RunFile(const Glib::ustring);
  void SetRunAction(const Glib::ustring);

  bool on_eventbox_button_press(GdkEventButton*, const Glib::ustring);
  Glib::ustring fullPath;
  int filesAtPath;
  //int heightAvailable;
  unsigned short iconmode;
  bool showHidden;
 // int x_pos, y_pos;

Gtk::Table * DisposableTable;
  void on_size_allocate(Gtk::Allocation&);
Gtk::EventBox ** sideconContainer;
int slotsUsed;
int IconsHigh;


  bool addSidecon(const Glib::ustring&, const Glib::RefPtr<const Gnome::Vfs::FileInfo>&, bool, bool&);
  bool addDetail(const Glib::ustring&, const Glib::RefPtr<const Gnome::Vfs::FileInfo>&, bool, bool&);

Gtk::Menu m_Menu_Popup;

public:

  DaIconModes(Glib::ustring, int, unsigned short, int, bool); //path, filesAtPath, mode 
  //void getNewTable(int);
  };

/**********************/

#endif //ICONMODES_H
