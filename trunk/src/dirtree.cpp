#include "dirtree.h"

dirtree::dirtree(Glib::ustring path){

  m_refTreeModel = Gtk::TreeStore::create(m_Columns);
  set_model(m_refTreeModel);


  //Fill the TreeView's model
  Gtk::TreeModel::Row row = *(m_refTreeModel->append());
  row[m_Columns.m_col_name] = "Billy Bob";

  Gtk::TreeModel::Row childrow = *(m_refTreeModel->append(row.children()));
  childrow[m_Columns.m_col_name] = "Billy Bob Junior";

  childrow = *(m_refTreeModel->append(row.children()));
  childrow[m_Columns.m_col_name] = "Sue Bob";

  row = *(m_refTreeModel->append());
  row[m_Columns.m_col_name] = "Joey Jojo";


  row = *(m_refTreeModel->append());
  row[m_Columns.m_col_name] = "Rob McRoberts";

  childrow = *(m_refTreeModel->append(row.children()));
  childrow[m_Columns.m_col_name] = "Xavier McRoberts";

  //Add the TreeView's view columns:
  append_column("Directory Tree", m_Columns.m_col_name);
  show();


}
