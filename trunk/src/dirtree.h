#ifndef DIRTREE_H
#define DIRTREE_H

#include <gtkmm/treemodel.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include "main.h"

class dirtree : public Gtk::TreeView {

  //Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    { /*add(m_col_id);*/ add(m_col_name); }

    //Gtk::TreeModelColumn<int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
  };
  ModelColumns m_Columns;



public:
  Glib::RefPtr<Gtk::TreeStore> m_refTreeModel;
  dirtree(Glib::ustring);
};

#endif // DIRTREE_H
