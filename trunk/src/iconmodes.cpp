/* Darimasen - iconmodes.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

#include "iconmodes.h"

/**********************/

void DaIconModes::proto_icon::run(){
  std::cout << path << FileName << "\n";

Glib::RefPtr<const Gnome::Vfs::FileInfo> info;
  Gnome::Vfs::Handle read_handle;
  try{
    read_handle.open( path + FileName, Gnome::Vfs::OPEN_READ);
    info = read_handle.get_file_info(
      Gnome::Vfs::FILE_INFO_GET_MIME_TYPE |
      Gnome::Vfs::FILE_INFO_FORCE_SLOW_MIME_TYPE );
    }
  catch(const Gnome::Vfs::exception& ex){
    std::cout << "Does not exist.\n";
    return;
    }

  Gnome::Vfs::Handle exec_handle;

  Glib::ustring exec = getenv("HOME");
  try{
    exec += "/Choices/MIME-types/";
    exec += FileMime.replace(FileMime.find("/"),1,"_");
    exec_handle.open(exec, Gnome::Vfs::OPEN_READ);

    exec += " \""  + path + FileName + "\"";
    Glib::spawn_command_line_async(exec);

    parent->parent->set_message(exec + " was run.");
    return;

    }
  catch(const Gnome::Vfs::exception& ex){
    }


  try{
    exec = exec.substr(0, exec.rfind("_"));
    exec_handle.open(exec, Gnome::Vfs::OPEN_READ);
    exec += " \""  + path + FileName + "\"";
    Glib::spawn_command_line_async(exec);

    parent->parent->set_message(exec + " was run.");
    return;
    }
  catch(const Gnome::Vfs::exception& ex){
    }

  try{
    if (Gnome::Vfs::Mime::can_be_executable(FileMime)){
      parent->parent->set_message( "Running " + path + FileName);
      Glib::spawn_command_line_async( path + FileName );
      return;
      }
   }
      catch(const Glib::Error) {}

  Gtk::MessageDialog dialog(
    "This mimetype does not have any action associated with it.\n"
    "should it be opened as text?",
    false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK_CANCEL);


  //Handle the response:
  switch(dialog.run())
  {
    case(Gtk::RESPONSE_OK):
    {
      Glib::ustring exec = getenv("HOME");
      exec += "/Choices/MIME-types/text";
      exec += " \""  + path + FileName + "\"";
      Glib::spawn_command_line_async(exec);
    parent->parent->set_message(exec + " was opened as a text file.");
      return;
      break;
    }
    default:
    {
    parent->parent->set_message("Well, that was usefull.");
      break;
    }
  }

  
  }

/**********************/

bool DaIconModes::proto_icon::select(GdkEventButton* event){

  if ((event->type == GDK_BUTTON_PRESS) && (event->button == 1)  && (parent->lastclick < event->time)){
    parent->lastclick = event->time + 1000;
    run();
    return true;
    }


  }

/**********************/

DaIconModes::proto_icon::proto_icon(
      DaIconModes& getParent,
      Glib::ustring getPath,
      const Glib::RefPtr<const Gnome::Vfs::FileInfo> getFile) {

  path = getPath;  parent = &getParent;

  icon = parent->getIcon(getFile->get_mime_type());

  FileName = getFile->get_name();

  if (getFile->get_name().length() > 25){
    int last = getFile->get_name().rfind(".");
    if (last != -1){
      ShortFileName = getFile->get_name().substr(0, 20) + ".." + getFile->get_name().substr(last);
      }
    else {
      ShortFileName = getFile->get_name().substr(0,22) + "...";
      }
    }
  else {
  ShortFileName = getFile->get_name();
  }

  FileMime = getFile->get_mime_type();

  guint size = getFile->get_size();
  if(size < 1024)
    FileSize = int2ustr(size) + " B";
  else if (size < 1048576)
    FileSize = int2ustr(size / 1024) + " KB";
  else
    FileSize = int2ustr(size / 1048576) + " MB";

  hidden = (getFile->get_name().substr(0,1) == ".");
  };

/**********************/

DaIconModes::proto_icon::~proto_icon(){
  std::cout << "proto deleted";
  }

/**********************/

DaIconModes::Sidecon::Sidecon(proto_icon & in){

  Arrange = new Gtk::Table();
  source = &in;

  Icon = new Gtk::Image(source->icon->scale_simple(48,48,Gdk::INTERP_TILES));
  Arrange->attach(*Icon, 0, 1, 0, 3, Gtk::FILL, Gtk::FILL, 0, 0);

  ShortName = new Gtk::Label(" " + source->ShortFileName);
  ShortName->set_alignment(0,0.5);
  Arrange->attach(*ShortName, 1, 2, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);

  Mime = new Gtk::Label(" " + source->FileMime);
  Mime->set_alignment(0,0.5);
  Arrange->attach(*Mime, 1, 2, 1, 2, Gtk::FILL, Gtk::AttachOptions(), 0, 0);

  Size = new Gtk::Label(" " + source->FileSize);
  Size->set_alignment(0,0.5);
  Arrange->attach(*Size, 1, 2, 2, 3, Gtk::FILL, Gtk::AttachOptions(), 0, 0);

  add(*Arrange);
  show_all_children();

  signal_button_press_event().connect(
    sigc::mem_fun(*source, &DaIconModes::proto_icon::select));

  }

/**********************/

DaIconModes::Sidecon::~Sidecon(){

  }

/**********************/

DaIconModes::Listview::Listview(proto_icon & in){

  source = &in;
  Arrange = new Gtk::Table();

  Icon = new Gtk::Image(source->icon->scale_simple(16,16,Gdk::INTERP_TILES));
  Arrange->attach(*Icon, 0, 1, 0, 1, Gtk::FILL, Gtk::FILL, 0, 0);

  Name = new Gtk::Label(" " + source->ShortFileName);
  Name->set_alignment(0,0.5);
  Arrange->attach(*Name, 1, 2, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);

  add(*Arrange);
  show_all_children();

  signal_button_press_event().connect(
    sigc::mem_fun(*source, &DaIconModes::proto_icon::select));

  }

/**********************/

DaIconModes::Listview::~Listview(){

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

  switch(mode){
    case 0:
      IconsHigh = allocation.get_height() / 57;
      break;
    case 1:
      IconsHigh = allocation.get_height() / 25;
      break;
    }
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
      switch(mode){
        case 0:
          Sidecon * tmpSidecon = new Sidecon(*iconlist[i]);
          DisposableTable->attach( *tmpSidecon, x_pos, x_pos+1, y_pos, y_pos+1,Gtk::FILL, Gtk::FILL, 4, 4);
          break;
        case 1:
          Listview * tmpListview = new Listview(*iconlist[i]);
          DisposableTable->attach( *tmpListview, x_pos, x_pos+1, y_pos, y_pos+1,Gtk::FILL, Gtk::FILL, 4, 4);
          break;
        }

      y_pos++; 
      if(parent->get_mode() == 0 || parent->get_mode() == 1){
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
