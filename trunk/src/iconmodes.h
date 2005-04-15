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
#include <unistd.h>
#include <gtkmm/liststore.h>

/**********************/

class DaIconModes : public Gtk::EventBox {

  class ListIcon : public Gtk::Table {
    Glib::ustring filePath, mimeInfo;
    DaIconModes * parent;
    Gtk::Image * image1;
    Gtk::Label * FileName;
  public:
    ListIcon(Glib::ustring, const Glib::RefPtr<const Gnome::Vfs::FileInfo>&, DaIconModes&);
    ~ ListIcon();
    };


  class Sidecon : public Gtk::Table {
    Glib::ustring filePath, mimeInfo;
    DaIconModes * parent;
    Gtk::Image * image1;
    Gtk::Label * FileName;
    Gtk::Label * FilePermissions ;
    Gtk::Label * FileSizeInfo;
  public:
    Sidecon(Glib::ustring, const Glib::RefPtr<const Gnome::Vfs::FileInfo>&, DaIconModes&);
    ~Sidecon();
    };

  class ModelColumns : public Gtk::TreeModel::ColumnRecord {
  public:

    ModelColumns(){
     // add(m_col_icon);
      add(m_col_name);
      add(m_col_size);
      add(m_col_mime);
      }

    //Gtk::TreeModelColumn<Gdk::Pixmap* > m_col_icon;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    Gtk::TreeModelColumn<Glib::ustring> m_col_size;
    Gtk::TreeModelColumn<Glib::ustring> m_col_mime;
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
//

  class Darimasen * parent;



  void RunFile(const Glib::ustring);
  void RunFileAsText(const Glib::ustring);

  void SetRunAction(const Glib::ustring);
  void SetPermissions(const Glib::ustring);

  void copy(const Glib::ustring);
  void move(const Glib::ustring);
  void link(const Glib::ustring);
  void unlinkify(const Glib::ustring);

  bool on_eventbox_button_press(GdkEventButton*, const Glib::ustring);
  void on_size_allocate(Gtk::Allocation&);
  void redraw();
  bool addEntry(Glib::ustring, Glib::RefPtr<const Gnome::Vfs::FileInfo>, bool, bool);
  Glib::RefPtr<Gdk::Pixbuf> getIcon(Glib::ustring, guint);



  Gtk::EventBox ** sideconContainer;

  ModelColumns * m_Columns;
  Gtk::TreeView m_TreeView;
  Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

  Gtk::Menu m_Menu_Popup;


//  unsigned short iconmode;
  guint32 lastclick;
  guint slotsUsed;
  guint IconsHigh;
  guint position;
  guint filesAtPath;
  int * hidden;
  short mode;

public:
  ~DaIconModes();
  DaIconModes(guint, Darimasen&);

  void SwitchHidden(guint);
  };

/**********************/

#endif //ICONMODES_H
