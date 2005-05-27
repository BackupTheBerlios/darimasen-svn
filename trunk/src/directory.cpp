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
  tree_view.set_model(tree_model);
  tree_view.append_column("Icon", icon_list.icon_pixmap); 
  tree_view.append_column("File Name", icon_list.file_name);



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
  
  
 // tree_model->set_sort_column(1,Gtk::SORT_ASCENDING);

  tree_model->set_sort_func(1,

      sigc::mem_fun(*this, &directory::sort_list_func));


  show();
}

bool directory::add_file(    Glib::ustring rel_path,
    Glib::RefPtr<const Gnome::Vfs::FileInfo> file,
    bool recursing_will_loop,
    bool recurse){


/*

  if (file->get_type() != Gnome::Vfs::FILE_TYPE_DIRECTORY){

  Gtk::TreeModel::Row row = *(tree_model->append());
std::cout << file->get_mime_type() << "\n";

Glib::ustring n = file->get_mime_type();
parent->get_icon(n.c_str());
  //row[icon_list.icon_pixmap] = parent->get_icon(file->get_mime_type());


  row[icon_list.file_name] = file->get_name();




    }
  else {
    Glib::RefPtr<Gnome::Vfs::Uri> x = Gnome::Vfs::Uri::create(
      parent->get_history(position) + file->get_name() + "/.DirIcon");
      if (x->uri_exists()){

  Gtk::TreeModel::Row row = *(tree_model->append());
//std::cout << file->get_mime_type() << "\n";

//Glib::ustring n = file->get_mime_type();
//parent->get_icon(n.c_str());
  //row[icon_list.icon_pixmap] = parent->get_icon(file->get_mime_type());


  row[icon_list.file_name] = file->get_name();


        }
    }

*/

  if (file->get_type() != Gnome::Vfs::FILE_TYPE_DIRECTORY ||
      Gnome::Vfs::Uri::create(rel_path + file->get_name() + "/.DirIcon")->uri_exists()){
    //file_list.push_back(*file);
    //std::cout << file_list.back()->get_name() << "\n";
  Gtk::TreeModel::Row row = *(tree_model->append());
    row[icon_list.file_name] = file->get_name();
    }

  return true;
  }


directory::~directory(){}

/*
void directory::render(){

  Gtk::TreeModel::Row row = *(tree_model->append());
  for(int i=0; i < file_list.size(); i++){
    std::cout << file_list[i]->get_name() << "\n";
    row[icon_list.file_name] = file_list[i]->get_name();
    row = *(tree_model->append());
    }


  }
*/



int directory::sort_list_func(const Gtk::TreeModel::iterator& A, const Gtk::TreeModel::iterator& B){
  
Glib::ustring n = (*A)->get_value(icon_list.file_name);
Glib::ustring b = (*A)->get_value(icon_list.file_name);
//std::cout << n <<"\n";
//Gtk::TreeModel::Row row = *(A);
//Gtk::TreeModel::Row row2 = *(B);


//std::cout <<  row[icon_list.file_name] << " " << row2[icon_list.file_name] << "\n";

  if(n.substr(0,1) == "." && b.substr(0,1) != ".")
    return -1;

  if(n.substr(0,1) != "." && b.substr(0,1) == ".")
    return 1;


  if(n > b){

    return -1;

    }

  if(n < b)
    return 1;

return 0;
}
