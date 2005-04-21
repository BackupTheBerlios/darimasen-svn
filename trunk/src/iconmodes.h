/* Darimasen - iconmodes.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef ICONMODES_H
#define ICONMODES_H


#include "darimasen.h"
#include <libgnomevfsmm.h>
#include <gtkmm/table.h>
#include <gdkmm/pixbuf.h>

/**********************/

class DaIconModes : public Gtk::EventBox {

  class proto_icon {
    Glib::ustring path;    DaIconModes * parent;

    void run();

  public:

    Glib::RefPtr<Gdk::Pixbuf> icon;
    Glib::ustring FileName;
    Glib::ustring ShortFileName;
    Glib::ustring FileMime;
    Glib::ustring FileSize;
    bool hidden;

    bool select(GdkEventButton*);

    proto_icon(DaIconModes&, Glib::ustring, const Glib::RefPtr<const Gnome::Vfs::FileInfo>);
    ~proto_icon();
    };

  class Sidecon : public Gtk::EventBox {
    proto_icon * source;

    Gtk::Table * Arrange;
    Gtk::Image * Icon;
    Gtk::Label * ShortName;
    Gtk::Label * Mime;
    Gtk::Label * Size;

  public:
    Sidecon(proto_icon&);
    ~Sidecon();
  };

  class Listview : public Gtk::EventBox {
    proto_icon * source;

    Gtk::Table * Arrange;
    Gtk::Image * Icon;
    Gtk::Label * Name;

  public:
    Listview(proto_icon&);
    ~Listview();
  };

  Glib::RefPtr<Gdk::Pixbuf> getIcon(Glib::ustring);
  bool addEntry(Glib::ustring, Glib::RefPtr<const Gnome::Vfs::FileInfo>, bool, bool);
  void on_size_allocate(Gtk::Allocation&);
  void redraw();

  class Darimasen * parent;
  guint position;
  guint32 lastclick;
  guint filesAtPath;
  gshort mode;
  proto_icon ** iconlist;
  guint slotsUsed;
  guint IconsHigh;


public:
  ~DaIconModes();
  DaIconModes(guint, class Darimasen&);

  void SwitchHidden(guint);
};

/**********************/

#endif //ICONMODES_H
