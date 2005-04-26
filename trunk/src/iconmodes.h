/* Darimasen - iconmodes.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef ICONMODES_H
#define ICONMODES_H


#include "darimasen.h"
#include <libgnomevfsmm.h>
#include <gtkmm/table.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/entry.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/separator.h>
#include <gtkmm/liststore.h>
#include <gtkmm/iconview.h>
/**********************/

class DaIconModes : public Gtk::EventBox {

  class proto_icon : public Gtk::TreeModel::ColumnRecord {
  public:

    proto_icon(){
      add(m_col_icon);
      add(m_col_name);
      add(m_col_size);
      add(m_col_mime);
      add(m_col_all);
      }

    Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > m_col_icon;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    Gtk::TreeModelColumn<Glib::ustring> m_col_size;
    Gtk::TreeModelColumn<Glib::ustring> m_col_mime;

    Gtk::TreeModelColumn<Glib::ustring> m_col_all;
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

    Gtk::CheckButton * u_r, * u_w, * u_x,* g_r, * g_w, * g_x, * o_r, * o_w, * o_x;
    
    Gnome::Vfs::Handle info;

    Gtk::VSeparator * extra;
    Gtk::CheckButton * sticky, * GID, * UID;

    Gtk::Label * user, * group, * others, * read, * write, * run, * explaination;

    Gtk::Table * layout;
    void cancled();
    void apply(Glib::RefPtr<Gnome::Vfs::FileInfo>);

    public:

    SetPermissionsDialogue(Glib::RefPtr<Gnome::Vfs::FileInfo> , Glib::ustring);
    ~SetPermissionsDialogue();
    };


  Glib::RefPtr<Gdk::Pixbuf> getIcon(Glib::ustring);
  bool addEntry(Glib::ustring, Glib::RefPtr<const Gnome::Vfs::FileInfo>, bool, bool);
  void on_size_allocate(Gtk::Allocation&);


  class Darimasen * parent;
  gint position;
  guint32 lastclick;
  gint filesAtPath;
  gshort mode;

  proto_icon * iconlist;
  Gtk::IconView m_TreeView;
  Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

void icon_selected(GdkEventButton*, Glib::ustring);

/*
int on_model_sort(const Gtk::TreeModel::iterator& a, const Gtk::TreeModel::iterator& b)
{
  ///* We need this function because we want to sort
   //* folders before files.
   

  Gtk::TreeModel::Row row_a = *a;
  Gtk::TreeModel::Row row_b = *b;
  
  const bool a_is_dir = row_a[iconlist->is_directory];
  const bool b_is_dir = row_b[m_columns.is_directory];
    
  if(!a_is_dir && b_is_dir)
    return 1;
  else if (a_is_dir && !b_is_dir)
    return -1;
  else
  {
    Glib::ustring name_a = row_a[m_columns.display_name];
    return name_a.compare( row_b[m_columns.display_name] );
  }
}
*/

public:
  ~DaIconModes();
  DaIconModes(guint, class Darimasen&);

  void SwitchHidden(guint);
};

/**********************/

#endif //ICONMODES_H
