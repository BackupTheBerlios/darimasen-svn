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


  class Sidecon : public Gtk::Table {
    Glib::ustring filePath, mimeInfo;
    public:
    Sidecon(Glib::ustring, const Glib::RefPtr<const Gnome::Vfs::FileInfo>& );
    };

/*
  class Detail : public Gtk::Table {
    public:
    Glib::ustring filePath, mimeInfo;
    Detail(Glib::ustring, const Glib::RefPtr<const Gnome::Vfs::FileInfo>& );
    };
*/

  class ChooseActionDialogue : public Gtk::Dialog {

   Gtk::RadioButton::Group _RadioBGroup_radiobutton1;
   Gtk::Button *cancelbutton1, *okbutton1;
   Gtk::RadioButton *radiobutton1, *radiobutton2;
   Gtk::Label       * label1;
   Gtk::Entry       * entry1;
   Gtk::VBox        * vbox1;

   Glib::ustring mime;
   void cancled();
   void modifyAction();

void GetCurrentAction(Glib::ustring);

    public:
    ChooseActionDialogue(Glib::ustring);
    };

  int * hidden;

  void RunFile(const Glib::ustring);
  void SetRunAction(const Glib::ustring);

  bool on_eventbox_button_press(GdkEventButton*, const Glib::ustring);
  Glib::ustring fullPath;
  int filesAtPath;
  unsigned short iconmode;
  bool showHidden;



  void on_size_allocate(Gtk::Allocation&);
  Gtk::EventBox ** sideconContainer;
  int slotsUsed;
  int IconsHigh;


  bool addEntry(const Glib::ustring&, const Glib::RefPtr<const Gnome::Vfs::FileInfo>&, bool, bool&);

Gtk::Menu m_Menu_Popup;

public:
~DaIconModes();
  DaIconModes(Glib::ustring, int, unsigned short, bool); //path, filesAtPath, mode 
  //void getNewTable(int);
  };

/**********************/

#endif //ICONMODES_H
