/* Darimasen - iconmodes.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef ICONMODES_H
#define ICONMODES_H

/**********************/

#include <gtkmm/table.h>
#include <gtkmm/messagedialog.h>
#include "main.h"
#include <libgnomevfsmm.h>
#include <gtkmm/entry.h>
#include <gdkmm/pixbuf.h>
#include "iconmodes.h"
#include "darimasen.h"
#include <gtkmm/separator.h>

/**********************/

class DaIconModes : public Gtk::EventBox {


  class Sidecon : public Gtk::Table {
    Glib::ustring filePath, mimeInfo;
    DaIconModes * parent;
    public:
    Sidecon(Glib::ustring, const Glib::RefPtr<const Gnome::Vfs::FileInfo>&, DaIconModes&);
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


  class SetPermissionsDialogue : public Gtk::Dialog {
    Glib::ustring fullPath;
    Gtk::Button * button1;
    Gtk::Button * button2;

    Gtk::CheckButton * u_r, * u_w, * u_x;
    Gtk::CheckButton * g_r, * g_w, * g_x;
    Gtk::CheckButton * o_r, * o_w, * o_x;
    
    Gnome::Vfs::Handle info;

    Gtk::VSeparator * extra;
    Gtk::CheckButton * sticky, * GID, * UID;

    Gtk::Label * user, * group, * others;
    Gtk::Label * read, * write, * run;
    Gtk::Label * explaination;

    Gtk::Table * layout;
    void cancled();
    void apply(Glib::RefPtr<Gnome::Vfs::FileInfo>);

    public:

    SetPermissionsDialogue(Glib::RefPtr<Gnome::Vfs::FileInfo> , Glib::ustring);
    };

  class Darimasen * parent;

 // int * hidden;

  void RunFile(const Glib::ustring);
  void SetRunAction(const Glib::ustring);
  void SetPermissions(const Glib::ustring);

  bool on_eventbox_button_press(GdkEventButton*, const Glib::ustring);
  void on_size_allocate(Gtk::Allocation&);
  void redraw();
  bool addEntry(const Glib::ustring&, const Glib::RefPtr<const Gnome::Vfs::FileInfo>&, bool, bool&);
  Glib::RefPtr<Gdk::Pixbuf> getIcon(Glib::ustring, guint);


  Glib::ustring fullPath;
  int filesAtPath;
  unsigned short iconmode;
  //bool showHidden;
  Gtk::EventBox ** sideconContainer;
  int slotsUsed;
  int IconsHigh;
  Gtk::Menu m_Menu_Popup;


public:
  ~DaIconModes();
  DaIconModes(Glib::ustring, Darimasen&);

  void SwitchHidden();
  };

/**********************/

#endif //ICONMODES_H
