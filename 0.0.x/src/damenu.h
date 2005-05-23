/* Darimasen - damenu.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef DAMENU_H
#define DAMENU_H

/**********************/

#include "main.h"
#include "darimasen.h"

/**********************/

class DarimasenMenu : public Gtk::MenuBar {

public:

  DarimasenMenu(const Glib::ustring&, class Darimasen&, guint);
  ~DarimasenMenu();

private:

  class Darimasen * parent;
  int depth;
  Gtk::Menu ** MenuArray; 
  Gtk::MenuItem ** MenuItemArray;
  Glib::ustring * menulevel;
  Gtk::Menu prompt;


  void bookmark(Glib::ustring);
  void copy(Glib::ustring);
  Glib::ustring CountSubdir(const Glib::ustring&);
  bool DaMenuSelect(GdkEventButton*, const Glib::ustring,guint,bool);
  void link(Glib::ustring);
  void MenuForPath(int, Glib::ustring , Glib::ustring);
  void move(Glib::ustring);
  bool * needsRebuild;
  bool SpecialMenuForPath(GdkEventButton* , int, Glib::ustring  , Glib::ustring);
  void selection_reset(guint, Glib::ustring);
  void unlinkify(Glib::ustring,gint);

public:

  void signal_deactivate ();

  };

/**********************/

#endif //DAMENU_H
