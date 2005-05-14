/* Darimasen - iconmodes.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef ICONMODES_H
#define ICONMODES_H


#include "main.h"
#include "darimasen.h"

/**********************/

class DaIconModes : public Gtk::EventBox {

  class proto_icon {
    Glib::ustring path;    DaIconModes * parent;

    void run() const;
    void runAsText() const;
    void SetRunAction() const;
    void SetPermissions() const;
    void copy();
    void move();
    void link();
    void unlinkify();

  public:

    Glib::RefPtr<Gdk::Pixbuf> icon;
    Glib::ustring FileName;
    Glib::ustring ShortFileName;
    Glib::ustring FileMime;
    Glib::ustring FileSize;
    bool hidden;

    void press_select(GdkEventButton*);
    void release_select(GdkEventButton*);

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
    ~ChooseActionDialogue(){

      delete cancelbutton1;
      delete okbutton1;
      delete label1;
      delete entry1;
      delete vbox1;
      delete radiobutton1;
      delete radiobutton2;
      }
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
    ~SetPermissionsDialogue();
    };

  class CopyDialogue : public Gtk::Dialog {
    class Gtk::Button * cancelbutton;
    class Gtk::Button * okbutton;
    class Gtk::Label * label;
    class Gtk::Entry * entry;
    void onOk();
    void onCancel();

    class Darimasen * grandparent;

  public:
    CopyDialogue(Glib::ustring, Darimasen&);
    ~CopyDialogue();
    };
//
  class LinkDialogue : public Gtk::Dialog {
    class Gtk::Button * cancelbutton;
    class Gtk::Button * okbutton;
    class Gtk::Label * label;
    class Gtk::Entry * entry;
    void onOk();
    void onCancel();

    class Darimasen * grandparent;

  public:
    LinkDialogue(Glib::ustring, Darimasen&);
    ~LinkDialogue();
    };
//
  class MoveDialogue : public Gtk::Dialog {
    class Gtk::Button * cancelbutton;
    class Gtk::Button * okbutton;
    class Gtk::Label * label;
    class Gtk::Entry * entry;
    void onOk();
    void onCancel();

    class Darimasen * grandparent;

  public:
    MoveDialogue(Glib::ustring, Darimasen&);
    ~MoveDialogue();
    };
//
  class DeleteDialogue : public Gtk::Dialog {
    class Gtk::Button * cancelbutton;
    class Gtk::Button * okbutton;
    class Gtk::Label * label;
    void onOk();
    void onCancel();

    class Darimasen * grandparent;

  public:
    DeleteDialogue(Glib::ustring, Darimasen&);
    ~DeleteDialogue();
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
  Gtk::Menu prompt;


public:
  ~DaIconModes();
  DaIconModes(guint, class Darimasen&);

  void SwitchHidden(guint);
};

/**********************/

#endif //ICONMODES_H
