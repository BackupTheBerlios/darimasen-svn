/* Darimasen - directory.cpp - Copyright (C) 2005 Sudrien, GPL */

#include "directory.h"

directory::directory(class tabs & ancestor, gint pos){

  modify_bg(Gtk::STATE_NORMAL, Gdk::Color("#ffffff"));

  parent = &ancestor;
  position = pos;

  // prepare the listing
  add(scroller);
  scroller.add(tree_view);
  tree_model = Gtk::ListStore::create(icon_list);

  tree_model_filtered = Gtk::TreeModelFilter::create( tree_model );


  tree_model_filtered->set_visible_column(icon_list.file_hidden);


  tree_view.set_model(tree_model_filtered);
tree_view.set_item_width(240);
tree_view.set_orientation(Gtk::ORIENTATION_HORIZONTAL);

  tree_view.set_pixbuf_column (icon_list.icon_pixmap);
  tree_view.set_text_column (icon_list.file_name);

/* TreeView
  tree_view.append_column("Icon", icon_list.icon_pixmap); 
  tree_view.append_column("File Name", icon_list.file_name);
  tree_view.append_column("Mime", icon_list.file_mime);
  tree_view.append_column("Size", icon_list.file_size);
  tree_view.append_column_numeric("Perm", icon_list.file_permissions, "%o" );
  tree_view.append_column("UID", icon_list.file_owner);
  tree_view.append_column("GID", icon_list.file_group);
*/

  tree_model->set_sort_column(1,Gtk::SORT_ASCENDING);
  tree_view.set_selection_mode(Gtk::SELECTION_MULTIPLE);

 // tree_selection = tree_view.get_selection();
 // tree_selection->set_mode(Gtk::SELECTION_MULTIPLE);

  //get the stuff for this directory
  try { // make all those files into proto_icons
    Gnome::Vfs::DirectoryHandle::visit(
      parent->get_history(position),
      Gnome::Vfs::FILE_INFO_GET_MIME_TYPE |
      Gnome::Vfs::FILE_INFO_FOLLOW_LINKS ,
      Gnome::Vfs::DIRECTORY_VISIT_LOOPCHECK,
      sigc::mem_fun(*this,&directory::add_file));
    }
  catch(const Gnome::Vfs::exception& ex){}
  
  tree_view.signal_item_activated().connect(sigc::mem_fun(*this,&directory::selection));
  tree_view.signal_button_press_event().connect(sigc::mem_fun(*this,&directory::on_press));


  show();
}




bool directory::add_file(Glib::ustring rel_path,
    Glib::RefPtr<const Gnome::Vfs::FileInfo> file,
    bool recursing_will_loop,
    bool recurse){
  if (file->get_type() != Gnome::Vfs::FILE_TYPE_DIRECTORY ||
      Gnome::Vfs::Uri::create(rel_path + file->get_name() + "/.DirIcon")->uri_exists()){
    Gtk::TreeModel::Row row = *(tree_model->append());

    row[icon_list.icon_pixmap] = parent->get_icon(file->get_mime_type());

    row[icon_list.file_name] = file->get_name();

    row[icon_list.file_mime] = file->get_mime_type();


    if(file->get_size() < 1024)
      row[icon_list.file_size] = int2ustr(file->get_size()) + " B";
    else if (file->get_size() < 1048576)
      row[icon_list.file_size] = int2ustr(file->get_size() / 1024) + " KB";
    else
      row[icon_list.file_size] = int2ustr(file->get_size() / 1048576) + " MB";

    row[icon_list.file_permissions] = file->get_permissions();
    row[icon_list.file_owner] = file->get_uid();
    row[icon_list.file_group] = file->get_gid();

    row[icon_list.file_hidden] = !(file->get_name().substr(0,1) == ".");

    }

  return true;
  }


directory::~directory(){}

void directory::selection(const Gtk::TreeModel::Path& path){
  std::cout << "clickie!\n";
  }

bool directory::on_press(GdkEventButton* event){
  std::cout << "clicked!\n";
  tree_view.unselect_all();
  return true;
  }
