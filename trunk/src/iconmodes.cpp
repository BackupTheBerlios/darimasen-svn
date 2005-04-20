/* Darimasen - iconmodes.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

#include "iconmodes.h"

/**********************/

DaIconModes::proto_icon::proto_icon(
      DaIconModes& getParent,
      Glib::ustring getPath,
      const Glib::RefPtr<const Gnome::Vfs::FileInfo> getFile) {

  path = getPath;  parent = &getParent;


  icon = parent->getIcon(getFile->get_mime_type());


  FileName = new Gtk::Label(getFile->get_name());
  FileName->set_alignment(0,0.5);
  FileName->show();

  if (getFile->get_name().length() > 25){
    int last = getFile->get_name().rfind(".");
    if (last != -1){
      ShortFileName = new Gtk::Label(
        getFile->get_name().substr(0, 20) + ".." + getFile->get_name().substr(last) );
      }
    else {
      ShortFileName = new Gtk::Label(getFile->get_name().substr(0,22) + "..." );
      }
    }
  else {
  ShortFileName = new Gtk::Label(getFile->get_name() );
  }
  ShortFileName->set_alignment(0,0.5);


  FileMime = new Gtk::Label(getFile->get_mime_type());
  FileMime->set_alignment(0,0.5);


  guint size = getFile->get_size();
  if(size < 1024)
    FileSize = new class Gtk::Label(int2ustr(size) + " B");
  else if (size < 1048576)
    FileSize = new class Gtk::Label(int2ustr(size / 1024) + " KB");
  else
    FileSize = new class Gtk::Label( int2ustr(size / 1048576) + " MB");
  FileSize->set_alignment(0,0.5);

  hidden = (getFile->get_name().substr(0,1) == ".");



  };

/**********************/


DaIconModes::proto_icon::~proto_icon(){
  delete FileName;
  delete FileMime;
  delete FileSize;
std::cout << "proto deleted";
  }

/**********************/

DaIconModes::Sidecon::Sidecon(proto_icon & in){

 // image1 = new class Gtk::Image(parent->getIcon(info->get_mime_type(), 48));
Gtk::Image * image = new Gtk::Image(in.icon->scale_simple(48,48,Gdk::INTERP_TILES));


  attach(*image, 0, 1, 0, 3, Gtk::FILL, Gtk::FILL, 0, 0);
  attach(*in.FileName, 1, 2, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
  attach(*in.FileMime, 1, 2, 1, 2, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
  attach(*in.FileSize, 1, 2, 2, 3, Gtk::FILL, Gtk::AttachOptions(), 0, 0);

  show_all_children();
  }

/**********************/

Glib::RefPtr<Gdk::Pixbuf> DaIconModes::getIcon(Glib::ustring mimeGiven){
  int i;
  static std::vector <Glib::ustring> mimeList;
  static std::vector < Glib::RefPtr<Gdk::Pixbuf> > unsizedImg;


  for(i = 0; i< mimeList.size(); i++){
    if( mimeGiven == mimeList[i] ){
      return unsizedImg[i];
      }
    }
   // pic doesn't exist.

    Glib::ustring ico;
  try{
    ico = "/usr/share/icons/gnome/48x48/mimetypes/gnome-mime-";
    ico += mimeGiven.replace(mimeGiven.find("/"), 1, "-");
    ico += ".png";

    Glib::file_get_contents(ico);
    }
  catch(const Glib::Error) {
    try{
      ico = "/usr/share/icons/gnome/48x48/mimetypes/gnome-mime-";
      ico += mimeGiven.substr(0,mimeGiven.find("-"));
      ico += ".png";
      Glib::file_get_contents(ico);
      }
    catch(const Glib::Error) {
     ico = "/usr/share/icons/gnome/48x48/filesystems/gnome-fs-loading-icon.png";
     }
    }

  mimeList.push_back(mimeGiven);

  Glib::RefPtr<Gdk::Pixbuf> xe = Gdk::Pixbuf::create_from_file(ico);

  unsizedImg.push_back(xe);

  return unsizedImg[i]; 
  }

/**********************/

bool DaIconModes::addEntry(
    Glib::ustring rel_path,
    Glib::RefPtr<const Gnome::Vfs::FileInfo> info,
    bool recursing_will_loop,
    bool recurse) {

  if (info->get_type() != Gnome::Vfs::FILE_TYPE_DIRECTORY){
    iconlist[slotsUsed++] = new proto_icon(*this, parent->history[position].top(), info);
    }
  return true;
  }

/**********************/

void DaIconModes::on_size_allocate(Gtk::Allocation& allocation){

  int oldie = IconsHigh;
  IconsHigh = allocation.get_height() / 58;
  if ( oldie != IconsHigh ){ //if resize is needed
    Gtk::Widget * tmp = get_child();

  redraw();
    }

  Gtk::EventBox::on_size_allocate(allocation);
  }

/**********************/

void DaIconModes::redraw(){
    Gtk::Widget * tmp = get_child();
  
   if (tmp)
        delete tmp; // actually DisposableTable, but segfaulted otherwise

      Gtk::Table * DisposableTable = new Gtk::Table((filesAtPath)/IconsHigh+1,IconsHigh);

      add(*DisposableTable);


      int y_pos = 0;
      int x_pos = 0;

      for(int i = 0; i < slotsUsed; i++){
        if(  !iconlist[i]->hidden || parent->optShowHidden->get_active()){

          Sidecon * tmpSidecon = new Sidecon(*iconlist[i]);



          DisposableTable->attach( *tmpSidecon, x_pos, x_pos+1, y_pos, y_pos+1,Gtk::FILL, Gtk::FILL, 4, 4);
          y_pos++; 
          if(parent->get_mode() == 0){
            if ( y_pos + 1 > IconsHigh){
              y_pos = 0;
              x_pos++;
              }
            }
          }
        }

      DisposableTable->show();
        DisposableTable->show_all_children();
  }

/**********************/

DaIconModes::DaIconModes(
      guint pos,
      Darimasen& myParent) {

  parent = &myParent;
  position = pos; //where we are in the world
  mode = parent->get_mode();

  lastclick = 0;  // a double-click detector. 
  filesAtPath = 0; // count files in directory

  try{  //count up the files in the directory. 
    Gnome::Vfs::DirectoryHandle handle;
    handle.open((parent->history[position].top()), Gnome::Vfs::FILE_INFO_DEFAULT);
    bool file_exists = true;
    while(file_exists){
      //if(  handle.read_next(file_exists)->get_type() != Gnome::Vfs::FILE_TYPE_DIRECTORY )
      handle.read_next(file_exists);
      filesAtPath++;
      }
    }
  catch(const Gnome::Vfs::exception&){}

  set_visible_window(false);

  iconlist = new proto_icon*[filesAtPath];
  slotsUsed = 0;
  IconsHigh = 0;
  try { // make all those files into proto_icons
    Gnome::Vfs::DirectoryHandle::visit(
      parent->history[position].top(),
      Gnome::Vfs::FILE_INFO_GET_MIME_TYPE |
      Gnome::Vfs::FILE_INFO_FORCE_FAST_MIME_TYPE |
      Gnome::Vfs::FILE_INFO_FOLLOW_LINKS ,
      Gnome::Vfs::DIRECTORY_VISIT_LOOPCHECK,
      sigc::mem_fun(*this,&DaIconModes::addEntry));
    }
  catch(const Gnome::Vfs::exception& ex){}

  }

/**********************/

DaIconModes::~DaIconModes(){

  }

/**********************/

 void DaIconModes::SwitchHidden(guint pos){

position = pos;
if (parent->optShowHidden->get_active())
    parent->set_message("Showing Hidden Files");
else
    parent->set_message("Hidden Files are hidden again");

  redraw();
  }

/**********************/
