/* Darimasen - iconmodes.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef ICONMODES_H
#define ICONMODES_H


#include "main.h"
#include "darimasen.h"

/**********************/

class DaIconModes : public Gtk::EventBox {

public:

  DaIconModes(guint, class Darimasen&);
  ~DaIconModes();

private:

  /****/

  class proto_icon {
  public:

    proto_icon(DaIconModes&, Glib::ustring, const Glib::RefPtr<const Gnome::Vfs::FileInfo>);
    ~proto_icon();

  private:

    Glib::ustring path;    DaIconModes * parent;

    void copy();
    void link();
    void move();
    void run() const;
    void runAsText() const;
    void setRunAction() const;
    void setPermissions() const;
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
    };

  /****/

  class Sidecon : public Gtk::EventBox {
  public:

    Sidecon(proto_icon&);
    ~Sidecon();

  private:
    proto_icon * source;
    Gtk::Table * Arrange;
    Gtk::Image * Icon;
    Gtk::Label * ShortName;
    Gtk::Label * Mime;
    Gtk::Label * Size;
  };

  /****/

  class Listview : public Gtk::EventBox {
  public:

    Listview(proto_icon&);
    ~Listview();

  private:

    proto_icon * source;
    Gtk::Table * Arrange;
    Gtk::Image * Icon;
    Gtk::Label * Name;
  };

  /****/

  class ChooseActionDialogue : public Gtk::Dialog {
  public:

    ChooseActionDialogue(Glib::ustring);
    ~ChooseActionDialogue();

  private:

    Gtk::RadioButton::Group _RadioBGroup_radiobutton1;
    Gtk::Button *cancelbutton1, *okbutton1;
    Gtk::RadioButton *radiobutton1, *radiobutton2;
    Gtk::Label       * label1;
    Gtk::Entry       * entry1;
    Gtk::VBox        * vbox1;
    Glib::ustring mime;
    void cancled();
    void getCurrentAction(Glib::ustring);
    void modifyAction();

    };

  /****/

  class setPermissionsDialogue : public Gtk::Dialog {

  public:

    setPermissionsDialogue(Glib::RefPtr<Gnome::Vfs::FileInfo> , Glib::ustring);
    ~setPermissionsDialogue();

  private:

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
    };

  /****/

  class CopyDialogue : public Gtk::Dialog {

  public:

    CopyDialogue(Glib::ustring, Darimasen&);
    ~CopyDialogue();

  private:

    class Darimasen * grandparent;

    class Gtk::Button * cancelbutton;
    class Gtk::Button * okbutton;
    class Gtk::Label * label;
    class Gtk::Entry * entry;

    void onCancel();
    void onOk();
    };

  /****/

  class LinkDialogue : public Gtk::Dialog {
  public:

    LinkDialogue(Glib::ustring, Darimasen&);
    ~LinkDialogue();

  private:

    class Gtk::Button * cancelbutton;
    class Gtk::Button * okbutton;
    class Gtk::Label * label;
    class Gtk::Entry * entry;
    void onOk();
    void onCancel();

    class Darimasen * grandparent;


    };

  /****/

  class MoveDialogue : public Gtk::Dialog {
  public:
    MoveDialogue(Glib::ustring, Darimasen&);
    ~MoveDialogue();

  private:

    class Gtk::Button * cancelbutton;
    class Gtk::Button * okbutton;
    class Gtk::Label * label;
    class Gtk::Entry * entry;
    void onOk();
    void onCancel();
    class Darimasen * grandparent;
    };

  /****/

  class DeleteDialogue : public Gtk::Dialog {
  public:
    DeleteDialogue(Glib::ustring, Darimasen&);
    ~DeleteDialogue();

  private:

    class Gtk::Button * cancelbutton;
    class Gtk::Button * okbutton;
    class Gtk::Label * label;
    void onOk();
    void onCancel();

    class Darimasen * grandparent;
    };

  /****/

  bool addEntry(Glib::ustring, Glib::RefPtr<const Gnome::Vfs::FileInfo>, bool, bool);
  void on_size_allocate(Gtk::Allocation&);
 // bool on_transfer_progress(const Gnome::Vfs::Transfer::ProgressInfo&);
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

  guint get_position();
  void SwitchHidden(guint);
  void update_position(guint);
};

/**********************/

#endif //ICONMODES_H
