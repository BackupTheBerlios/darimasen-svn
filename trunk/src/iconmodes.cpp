/* Darimasen - iconmodes.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

#include "iconmodes.h"

/**********************/

void DaIconModes::proto_icon::run() const{

  Glib::RefPtr<Gnome::Vfs::Uri> x = Gnome::Vfs::Uri::create(path + FileName);

  if (!x->uri_exists()){
    std::cout << "Is not a file - ";
    x = Gnome::Vfs::Uri::create(path);
    if (!x->uri_exists()){
      std::cout << "is not an appdir either. Bogus.\n";
      }
    else {
      std::cout << "it is a appdir.\n";
    Glib::spawn_command_line_async(path + "AppRun");

    parent->parent->set_message(path + " was executed.");
	return;
      }
    }

  Gnome::Vfs::Handle exec_handle;

  Glib::ustring exec = getenv("HOME");
  try{
    exec += "/Choices/MIME-types/";
    Glib::ustring tmp = FileMime;
    exec += tmp.replace(tmp.find("/"),1,"_");
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
  switch(dialog.run()) {
    case(Gtk::RESPONSE_OK):
      {
        Glib::ustring exec = getenv("HOME");
        exec += "/Choices/MIME-types/text";
        exec += " \""  + path + FileName + "\"";
        Glib::spawn_command_line_async(exec);
        parent->parent->set_message(exec + " was opened as a text file.");
        return;
        }
      break;
    default:
      parent->parent->set_message("Well, that was usefull.");
      break;
    }
  }

/**********************/

void DaIconModes::proto_icon::runAsText() const{

  // double checking the file exists
  Glib::RefPtr<const Gnome::Vfs::FileInfo> info;
  Gnome::Vfs::Handle read_handle;
  try{
    read_handle.open( path + FileName, Gnome::Vfs::OPEN_READ);
    info = read_handle.get_file_info(
      Gnome::Vfs::FILE_INFO_GET_MIME_TYPE );
    }
  catch(const Gnome::Vfs::exception& ex){
    std::cout << "Does not exist.\n";
    return;
    }

  Gnome::Vfs::Handle exec_handle;
  Glib::ustring exec = getenv("HOME");
  try{
    exec += "/Choices/MIME-types/text";
    exec_handle.open(exec, Gnome::Vfs::OPEN_READ);
    exec += " \""  + path + FileName + "\"";
    Glib::spawn_command_line_async(exec);

    parent->parent->set_message(exec + " was run.");
    return;
    }
  catch(const Gnome::Vfs::exception& ex){}
  }

/**********************/

void DaIconModes::proto_icon::SetRunAction() const{

  Gnome::Vfs::Handle read_handle;
  Glib::RefPtr<const Gnome::Vfs::FileInfo> info;
  Glib::ustring exec_subtype;
  Glib::ustring exec_mimetype;

  try {
    read_handle.open( path + FileName, Gnome::Vfs::OPEN_READ);
    info = read_handle.get_file_info(
        Gnome::Vfs::FILE_INFO_GET_MIME_TYPE |
        Gnome::Vfs::FILE_INFO_FORCE_SLOW_MIME_TYPE );

    exec_subtype  = info->get_mime_type();
    exec_subtype  = exec_subtype.replace( exec_subtype.find("/"), 1, "_" );
    exec_mimetype = exec_subtype.substr( 0, exec_subtype.rfind("_") );

    ChooseActionDialogue * chooseAction;
    chooseAction = new ChooseActionDialogue(exec_subtype);
    chooseAction->show();
    }
  catch(const Gnome::Vfs::exception& ex) {
    parent->parent->set_message("Err... Setting error?");
    }
  }

/**********************/

void DaIconModes::proto_icon::SetPermissions() const{


  Gnome::Vfs::Handle read_handle;
  Glib::RefPtr<Gnome::Vfs::FileInfo> info;

  try {
    read_handle.open( path + FileName, Gnome::Vfs::OPEN_READ);
    info = read_handle.get_file_info(Gnome::Vfs::FILE_INFO_GET_ACCESS_RIGHTS);

    SetPermissionsDialogue * setPermissions;
    setPermissions = new SetPermissionsDialogue(info,path);
    setPermissions->show();
    }
  catch(const Gnome::Vfs::exception& ex) {
    parent->parent->set_message("Err... Setting error?");
    }
  }

/**********************/

void DaIconModes::proto_icon::release_select(GdkEventButton* event){

  if ((event->type == GDK_BUTTON_RELEASE) && (event->button == 1)  && (parent->lastclick < event->time)){
    parent->lastclick = event->time + 1000;
    run();
    }
  }

/**********************/

void DaIconModes::proto_icon::press_select(GdkEventButton* event){
  if ((event->type == GDK_BUTTON_PRESS) && (event->button == 3)){

    for( int i=10; i > 0; i--)
      parent->prompt.items().pop_back();

    parent->prompt.items().push_back(
      Gtk::Menu_Helpers::MenuElem("Open \"" + FileName + "\"",
        sigc::mem_fun(*this,&DaIconModes::proto_icon::run)));

    parent->prompt.items().push_back(
      Gtk::Menu_Helpers::MenuElem("Open as text ",
        sigc::mem_fun(*this, &DaIconModes::proto_icon::runAsText)));

    parent->prompt.items().push_back(
      Gtk::Menu_Helpers::SeparatorElem());

    parent->prompt.items().push_back(
      Gtk::Menu_Helpers::MenuElem("Set Run Action...",
        sigc::mem_fun(*this, &DaIconModes::proto_icon::SetRunAction)));

    parent->prompt.items().push_back(
      Gtk::Menu_Helpers::MenuElem("Permissions...",
        sigc::mem_fun(*this, &DaIconModes::proto_icon::SetPermissions)));


    parent->prompt.items().push_back( Gtk::Menu_Helpers::SeparatorElem());

    parent->prompt.items().push_back(
      Gtk::Menu_Helpers::MenuElem("Copy To...",
        sigc::mem_fun(*this, &DaIconModes::proto_icon::copy)));

    parent->prompt.items().push_back(
      Gtk::Menu_Helpers::MenuElem("Move To...",
        sigc::mem_fun(*this, &DaIconModes::proto_icon::move)));

    parent->prompt.items().push_back(
      Gtk::Menu_Helpers::MenuElem("Link To...",
        sigc::mem_fun(*this, &DaIconModes::proto_icon::link)));

    parent->prompt.items().push_back(
      Gtk::Menu_Helpers::MenuElem("Delete ",
        sigc::mem_fun(*this, &DaIconModes::proto_icon::unlinkify)));



 // Glib::RefPtr<Gnome::Vfs::Uri> x = Gnome::Vfs::Uri::create( path + ".DirIcon");
    if (FileMime != "AppDir"){
      parent->prompt.popup(event->button, event->time);
      }
    }
  }

/**********************/

DaIconModes::proto_icon::proto_icon(
      DaIconModes& getParent,
      Glib::ustring getPath,
      const Glib::RefPtr<const Gnome::Vfs::FileInfo> getFile) {

  path = getPath;
  parent = &getParent;

  Glib::RefPtr<Gnome::Vfs::Uri> x = Gnome::Vfs::Uri::create(
    getPath + ".DirIcon");
  if (x->uri_exists()){
    icon = Gdk::Pixbuf::create_from_file(getPath + ".DirIcon");
    FileMime = "AppDir";
    }
  else {

    icon = parent->getIcon(getFile->get_mime_type());
    FileMime = getFile->get_mime_type();
    }

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

  signal_button_press_event().connect_notify(
    sigc::mem_fun(*source, &DaIconModes::proto_icon::press_select));
  signal_button_release_event().connect_notify(
    sigc::mem_fun(*source, &DaIconModes::proto_icon::release_select));

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

  signal_button_press_event().connect_notify(
    sigc::mem_fun(*source, &DaIconModes::proto_icon::press_select));
  signal_button_release_event().connect_notify(
    sigc::mem_fun(*source, &DaIconModes::proto_icon::release_select));

  }

/**********************/

DaIconModes::Listview::~Listview(){

  }

/**********************/

DaIconModes::ChooseActionDialogue::ChooseActionDialogue(Glib::ustring mimeType){

  mime = mimeType;
  cancelbutton1 = new class Gtk::Button(Gtk::StockID("gtk-close"));
  okbutton1 = new class Gtk::Button(Gtk::StockID("gtk-apply"));
  label1 = new class Gtk::Label("Enter a Shell command:");
  entry1 = new class Gtk::Entry();
  vbox1  = new class Gtk::VBox(false, 0);


  radiobutton1 = new class Gtk::RadioButton(_RadioBGroup_radiobutton1, 
    "Set Action For \"" + mimeType.substr(0, mimeType.find("_")) + "\"" );

  radiobutton1->signal_clicked().connect(
      sigc::bind<Glib::ustring >( sigc::mem_fun(*this,
      &DaIconModes::ChooseActionDialogue::GetCurrentAction), mimeType.substr(0, mimeType.find("_")) ));

  radiobutton2 = new class Gtk::RadioButton(_RadioBGroup_radiobutton1,
      "Set Action For \"" + mimeType + "\"")  ;

  radiobutton2->signal_clicked().connect(
      sigc::bind<Glib::ustring >( sigc::mem_fun(*this,
      &DaIconModes::ChooseActionDialogue::GetCurrentAction), mimeType ));

  cancelbutton1->set_flags(Gtk::CAN_FOCUS);
  cancelbutton1->set_flags(Gtk::CAN_DEFAULT);
  cancelbutton1->signal_clicked().connect(
      sigc::mem_fun(*this, &DaIconModes::ChooseActionDialogue::cancled) );
  okbutton1->set_flags(Gtk::CAN_FOCUS);
  okbutton1->set_flags(Gtk::CAN_DEFAULT);
  okbutton1->signal_clicked().connect(
      sigc::mem_fun(*this, &DaIconModes::ChooseActionDialogue::modifyAction) );
  get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
  radiobutton1->set_flags(Gtk::CAN_FOCUS);
  radiobutton1->set_mode(true);
  radiobutton1->set_active(false);
  radiobutton2->set_flags(Gtk::CAN_FOCUS);
  radiobutton2->set_mode(true);
  radiobutton2->set_active(true);
  label1->set_padding(2,4);
  label1->set_justify(Gtk::JUSTIFY_LEFT);
  entry1->set_flags(Gtk::CAN_FOCUS);
  entry1->set_has_frame(true);
  vbox1->pack_start(*radiobutton1, Gtk::PACK_SHRINK, 0);
  vbox1->pack_start(*radiobutton2, Gtk::PACK_SHRINK, 0);
  vbox1->pack_start(*label1, Gtk::PACK_SHRINK, 0);
  vbox1->pack_start(*entry1, Gtk::PACK_SHRINK, 0);
  get_vbox()->set_homogeneous(false);
  get_vbox()->set_spacing(5);
  get_vbox()->set_border_width(6);
  get_vbox()->pack_start(*vbox1);
  set_border_width(6);
  set_title("Set Run Action");
  set_modal(true);
  property_window_position().set_value(Gtk::WIN_POS_CENTER_ON_PARENT);
  set_resizable(true);
  property_destroy_with_parent().set_value(false);
  set_has_separator(false);
  add_action_widget(*cancelbutton1, Gtk::RESPONSE_CANCEL);
  add_action_widget(*okbutton1, Gtk::RESPONSE_OK);
  set_default_response(Gtk::RESPONSE_OK);

  show_all_children();
  }

/**********************/

void DaIconModes::ChooseActionDialogue::GetCurrentAction(Glib::ustring mimeType){

  Glib::ustring contents, exec1 = getenv("HOME");
  exec1 += (Glib::ustring)("/Choices/MIME-types/");
  exec1 += mimeType;

  try {
    contents = Glib::file_get_contents(exec1);
    contents = contents.substr(contents.find("exec ") + 5);
    contents = contents.substr(0,contents.find("\n"));
    }
  catch(const Glib::Error) {
    contents = "* \"$@\"" ;
    }
  entry1->set_text(contents);
}

/**********************/

void DaIconModes::ChooseActionDialogue::cancled(){
  hide();
  return;
  }

/**********************/

void DaIconModes::ChooseActionDialogue::modifyAction(){

  if ( entry1->get_text() != "* \"$@\""){ //don't intentionally make a broken script.
    Glib::ustring command = "#! /bin/sh\nexec ";
    command += entry1->get_text();
    command += "\n";

    try {
      Glib::ustring exec1 = getenv("HOME");
      exec1 += (Glib::ustring)("/Choices/MIME-types/");
      if(radiobutton2->get_active()) exec1 += mime;
      if(radiobutton1->get_active())exec1 += mime.substr(0, mime.find("_"));
      Gnome::Vfs::Handle write_handle;

      // Now write the data we read out to the output uri
      write_handle.create(exec1, Gnome::Vfs::OPEN_WRITE, false, 0755);
      write_handle.seek(Gnome::Vfs::SEEK_POS_START, 0);
      GnomeVFSFileSize bytes_written = write_handle.write(command.data(), command.size());
      }
    catch(const Gnome::Vfs::exception) {
  //  parent->set_message("Couldn't write new definition.");
      hide();
      return;
      }
    }
  hide();
  return;
  }

/**********************/

DaIconModes::SetPermissionsDialogue::SetPermissionsDialogue(
    Glib::RefPtr<Gnome::Vfs::FileInfo> info, Glib::ustring path){
fullPath = path;
  set_title("Set file permissions");
  set_modal(true);

   read = new class Gtk::Label("R");
   write = new class Gtk::Label("W");
   run = new class Gtk::Label("X");
   user = new class Gtk::Label("User");
   group = new class Gtk::Label("Group");
   others = new class Gtk::Label("Everybody");

   explaination = new class Gtk::Label("for \""+ info->get_name() + "\"...");

   layout = new class Gtk::Table(5, 6, false);

   u_r = new class Gtk::CheckButton("");
   if((info->get_permissions() & Gnome::Vfs::PERM_USER_READ) != 0)  u_r->set_active(true);
   u_w = new class Gtk::CheckButton("");
   if((info->get_permissions() & Gnome::Vfs::PERM_USER_WRITE) != 0)  u_w->set_active(true);
   u_x = new class Gtk::CheckButton("");
   if((info->get_permissions() & Gnome::Vfs::PERM_USER_EXEC) != 0)  u_x->set_active(true);
   g_r = new class Gtk::CheckButton("");
   if((info->get_permissions() & Gnome::Vfs::PERM_GROUP_READ) != 0)  g_r->set_active(true);
   g_w = new class Gtk::CheckButton("");
   if((info->get_permissions() & Gnome::Vfs::PERM_GROUP_WRITE) != 0)  g_w->set_active(true);
   g_x = new class Gtk::CheckButton("");
   if((info->get_permissions() & Gnome::Vfs::PERM_GROUP_EXEC) != 0)  g_x->set_active(true);
   o_r = new class Gtk::CheckButton("");
   if((info->get_permissions() & Gnome::Vfs::PERM_OTHER_READ) != 0)  o_r->set_active(true);
   o_w = new class Gtk::CheckButton("");
   if((info->get_permissions() & Gnome::Vfs::PERM_OTHER_WRITE) != 0)  o_w->set_active(true);
   o_x = new class Gtk::CheckButton("");
   if((info->get_permissions() & Gnome::Vfs::PERM_OTHER_EXEC) != 0)  o_x->set_active(true);

   extra = new  Gtk::VSeparator;

   sticky = new class Gtk::CheckButton("Sticky");
   if((info->get_permissions() & Gnome::Vfs::PERM_STICKY) != 0)  sticky->set_active(true);
   GID = new class Gtk::CheckButton("Group ID");
   if((info->get_permissions() & Gnome::Vfs::PERM_SGID) != 0)  GID->set_active(true);
   UID = new class Gtk::CheckButton("User ID");
   if((info->get_permissions() & Gnome::Vfs::PERM_SUID) != 0)  UID->set_active(true);

   layout->attach(*u_r, 1, 2, 2, 3, Gtk::FILL, Gtk::FILL, 0, 0);
   layout->attach(*u_w, 2, 3, 2, 3, Gtk::FILL, Gtk::FILL, 0, 0);
   layout->attach(*u_x, 3, 4, 2, 3, Gtk::FILL, Gtk::FILL, 0, 0);
   layout->attach(*g_r, 1, 2, 3, 4, Gtk::FILL, Gtk::FILL, 0, 0);
   layout->attach(*g_w, 2, 3, 3, 4, Gtk::FILL, Gtk::FILL, 0, 0);
   layout->attach(*g_x, 3, 4, 3, 4, Gtk::FILL, Gtk::FILL, 0, 0);
   layout->attach(*o_r, 1, 2, 4, 5, Gtk::FILL, Gtk::FILL, 0, 0);
   layout->attach(*o_w, 2, 3, 4, 5, Gtk::FILL, Gtk::FILL, 0, 0);
   layout->attach(*o_x, 3, 4, 4, 5, Gtk::FILL, Gtk::FILL, 0, 0);

   layout->attach(*extra,4,5,1,5, Gtk::FILL | Gtk::EXPAND, Gtk::FILL, 10, 0);

   layout->attach(*sticky, 5, 6, 2, 3, Gtk::FILL, Gtk::FILL, 0, 0);
   layout->attach(*GID, 5, 6, 3, 4, Gtk::FILL, Gtk::FILL, 0, 0);
   layout->attach(*UID, 5, 6, 4, 5, Gtk::FILL, Gtk::FILL, 0, 0);

   layout->attach(*read, 1, 2, 1, 2, Gtk::FILL, Gtk::FILL, 0, 0);
   layout->attach(*write, 2, 3, 1, 2, Gtk::FILL, Gtk::FILL, 0, 0);
   layout->attach(*run, 3, 4, 1, 2, Gtk::FILL, Gtk::FILL, 0, 0);
   layout->attach(*user, 0, 1, 2, 3, Gtk::FILL , Gtk::FILL, 0, 0);
   layout->attach(*group, 0, 1, 3, 4, Gtk::FILL , Gtk::FILL, 0, 0);
   layout->attach(*others, 0, 1, 4, 5, Gtk::FILL , Gtk::FILL, 0, 0);
   layout->attach(*explaination, 0, 6, 0, 1, Gtk::FILL | Gtk::EXPAND, Gtk::FILL, 10, 0);


  get_vbox()->pack_start(*layout);
  button1 = new class Gtk::Button(Gtk::StockID("gtk-cancel"));
  button1->signal_clicked().connect(
      sigc::mem_fun(*this, &DaIconModes::SetPermissionsDialogue::cancled) );


  button2 = new class Gtk::Button(Gtk::StockID("gtk-apply"));
  button2->signal_clicked().connect(
    sigc::bind<Glib::RefPtr<Gnome::Vfs::FileInfo> >(
      sigc::mem_fun(*this, &DaIconModes::SetPermissionsDialogue::apply), info ));


  get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
  add_action_widget(*button1, -6);
  add_action_widget(*button2, -10);

  set_modal(true);
  show_all_children();
  show();
  }

/**********************/

void DaIconModes::SetPermissionsDialogue::cancled(){
  hide();
  }

/**********************/

void DaIconModes::SetPermissionsDialogue::apply(Glib::RefPtr<Gnome::Vfs::FileInfo> info){

  // *nix and *BSD only, blah blah blah.
  int permissions = 0;



  if(UID->get_active()) permissions += 2048;
  if(GID->get_active()) permissions += 1024;
  if(sticky->get_active()) permissions += 512;

  if(u_r->get_active()) permissions += 256;
  if(u_w->get_active()) permissions += 128;
  if(u_x->get_active()) permissions += 64;

  if(g_r->get_active()) permissions += 32;
  if(g_w->get_active()) permissions += 16;
  if(g_x->get_active()) permissions += 8;

  if(o_r->get_active()) permissions += 4;
  if(o_w->get_active()) permissions += 2;
  if(o_x->get_active()) permissions += 1;

  chmod((fullPath + info->get_name()).c_str(), permissions);

  hide();
  }

/**********************/

DaIconModes::SetPermissionsDialogue::~SetPermissionsDialogue(){
      delete read;
      delete write;
      delete run;
      delete user;
      delete group;
      delete others;
      delete explaination;
      delete layout;
      delete u_r;
      delete u_w;
      delete u_x;
      delete g_r;
      delete g_w;
      delete g_x;
      delete o_r;
      delete o_w;
      delete o_x;
      delete extra;
      delete sticky;
      delete GID;
      delete UID;
      delete button1;
      delete button2;
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
    iconlist[slotsUsed++] = new proto_icon(*this, parent->get_history(position), info);
    }
  else {
    Glib::RefPtr<Gnome::Vfs::Uri> x = Gnome::Vfs::Uri::create(
      parent->get_history(position) + info->get_name() + slash + ".DirIcon");
      if (x->uri_exists()){
        //std::cout<< x->get_path() << "\n";
        iconlist[slotsUsed++] = new proto_icon(
          *this, parent->get_history(position) + info->get_name() + slash, info);
        }
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
        case 0: {
          Sidecon * tmpSidecon = new Sidecon(*iconlist[i]);
          DisposableTable->attach( *tmpSidecon, x_pos, x_pos+1, y_pos, y_pos+1,Gtk::FILL, Gtk::FILL, 4, 4);
            }
          break;
        case 1: {
          Listview * tmpListview = new Listview(*iconlist[i]);
          DisposableTable->attach( *tmpListview, x_pos, x_pos+1, y_pos, y_pos+1,Gtk::FILL, Gtk::FILL, 4, 4);
            }
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
    handle.open(parent->get_history(position), Gnome::Vfs::FILE_INFO_DEFAULT);
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
      parent->get_history(position),
      Gnome::Vfs::FILE_INFO_GET_MIME_TYPE |
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
/**********************/

DaIconModes::CopyDialogue::CopyDialogue(Glib::ustring file, Darimasen& parent){

grandparent = &parent;
   cancelbutton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
   okbutton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   label = Gtk::manage(new class Gtk::Label(file));
   entry = Gtk::manage(new class Gtk::Entry());

   okbutton->set_flags(Gtk::CAN_FOCUS);
   okbutton->set_flags(Gtk::CAN_DEFAULT);
   cancelbutton->set_flags(Gtk::CAN_FOCUS);

   set_title("Copy File to");
   set_modal(true);

   get_vbox()->pack_start(*label, Gtk::PACK_SHRINK, 0);
   get_vbox()->pack_start(*entry, Gtk::PACK_SHRINK, 0);


   entry->set_text(file);

   add_action_widget(*cancelbutton, -6);
   add_action_widget(*okbutton, -5);


   okbutton->signal_clicked().connect_notify(sigc::mem_fun(*this, &DaIconModes::CopyDialogue::onOk));
   cancelbutton->signal_clicked().connect_notify(sigc::mem_fun(*this, &DaIconModes::CopyDialogue::onCancel));


   show_all_children();
  }

/**********************/

void DaIconModes::CopyDialogue::onOk(){
  Glib::spawn_command_line_async((Glib::ustring)"cp \""  + label->get_text().c_str() + (Glib::ustring)"\" \"" + entry->get_text().c_str()+ "\"");

  grandparent->updateView(
    label->get_text().substr(0, label->get_text().rfind(slash,label->get_text().length())+1),
    entry->get_text().substr(0, entry->get_text().rfind(slash,entry->get_text().length())+1));

  hide();
  }

/**********************/

void DaIconModes::CopyDialogue::onCancel(){
  hide();
  }

/**********************/

DaIconModes::CopyDialogue::~CopyDialogue(){}

/**********************/
/**********************/

DaIconModes::MoveDialogue::MoveDialogue(Glib::ustring file, Darimasen& parent){

grandparent = &parent;
   cancelbutton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
   okbutton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   label = Gtk::manage(new class Gtk::Label(file));
   entry = Gtk::manage(new class Gtk::Entry());

   okbutton->set_flags(Gtk::CAN_FOCUS);
   okbutton->set_flags(Gtk::CAN_DEFAULT);
   cancelbutton->set_flags(Gtk::CAN_FOCUS);

   set_title("Move File to");
   set_modal(true);

   get_vbox()->pack_start(*label, Gtk::PACK_SHRINK, 0);
   get_vbox()->pack_start(*entry, Gtk::PACK_SHRINK, 0);


   entry->set_text(file);

   add_action_widget(*cancelbutton, -6);
   add_action_widget(*okbutton, -5);


   okbutton->signal_clicked().connect_notify(sigc::mem_fun(*this, &DaIconModes::MoveDialogue::onOk));
   cancelbutton->signal_clicked().connect_notify(sigc::mem_fun(*this, &DaIconModes::MoveDialogue::onCancel));


   show_all_children();
  }

/**********************/

void DaIconModes::MoveDialogue::onOk(){
  Glib::spawn_command_line_async((Glib::ustring)"mv \""  + label->get_text().c_str() + (Glib::ustring)"\" \"" + entry->get_text().c_str()+ "\"");

  grandparent->updateView(
    label->get_text().substr(0, label->get_text().rfind(slash,label->get_text().length())+1),
    entry->get_text().substr(0, entry->get_text().rfind(slash,entry->get_text().length())+1));

  hide();
  }

/**********************/

void DaIconModes::MoveDialogue::onCancel(){
  hide();
  }

/**********************/

DaIconModes::MoveDialogue::~MoveDialogue(){}

/**********************/
/**********************/

DaIconModes::LinkDialogue::LinkDialogue(Glib::ustring file, Darimasen& parent){

grandparent = &parent;
   cancelbutton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
   okbutton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   label = Gtk::manage(new class Gtk::Label(file));
   entry = Gtk::manage(new class Gtk::Entry());

   okbutton->set_flags(Gtk::CAN_FOCUS);
   okbutton->set_flags(Gtk::CAN_DEFAULT);
   cancelbutton->set_flags(Gtk::CAN_FOCUS);

   set_title("Link File to");
   set_modal(true);

   get_vbox()->pack_start(*label, Gtk::PACK_SHRINK, 0);
   get_vbox()->pack_start(*entry, Gtk::PACK_SHRINK, 0);


   entry->set_text(file);

   add_action_widget(*cancelbutton, -6);
   add_action_widget(*okbutton, -5);


   okbutton->signal_clicked().connect_notify(sigc::mem_fun(*this, &DaIconModes::LinkDialogue::onOk));
   cancelbutton->signal_clicked().connect_notify(sigc::mem_fun(*this, &DaIconModes::LinkDialogue::onCancel));


   show_all_children();
  }

/**********************/

void DaIconModes::LinkDialogue::onOk(){
  Glib::spawn_command_line_async((Glib::ustring)"ln -s \""  + label->get_text().c_str() + (Glib::ustring)"\" \"" + entry->get_text().c_str() + "\"");

  grandparent->updateView(
    label->get_text().substr(0, label->get_text().rfind(slash,label->get_text().length())+1),
    entry->get_text().substr(0, entry->get_text().rfind(slash,entry->get_text().length())+1));

  hide();
  }

/**********************/

void DaIconModes::LinkDialogue::onCancel(){
  hide();
  }

/**********************/

DaIconModes::LinkDialogue::~LinkDialogue(){}

/**********************/
/**********************/

DaIconModes::DeleteDialogue::DeleteDialogue(Glib::ustring file, Darimasen& parent){

grandparent = &parent;
   cancelbutton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
   okbutton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   label = Gtk::manage(new class Gtk::Label(file));

   okbutton->set_flags(Gtk::CAN_FOCUS);
   okbutton->set_flags(Gtk::CAN_DEFAULT);
   cancelbutton->set_flags(Gtk::CAN_FOCUS);

   set_title("Delete File");
   set_modal(true);

   get_vbox()->pack_start(*label, Gtk::PACK_SHRINK, 0);

   add_action_widget(*cancelbutton, -6);
   add_action_widget(*okbutton, -5);


   okbutton->signal_clicked().connect_notify(sigc::mem_fun(*this, &DaIconModes::DeleteDialogue::onOk));
   cancelbutton->signal_clicked().connect_notify(sigc::mem_fun(*this, &DaIconModes::DeleteDialogue::onCancel));


   show_all_children();
  }

/**********************/

void DaIconModes::DeleteDialogue::onOk(){
  Glib::spawn_command_line_async((Glib::ustring)"rm \""  + label->get_text().c_str() + "\"");

  grandparent->updateView(
    label->get_text().substr(0, label->get_text().rfind(slash,label->get_text().length())+1),
    label->get_text().substr(0, label->get_text().rfind(slash,label->get_text().length())+1));
   //std::cout << label->get_text().substr(0, label->get_text().rfind(slash,label->get_text().length())+1) ;
  hide();
  }

/**********************/

void DaIconModes::DeleteDialogue::onCancel(){
  hide();
  }

/**********************/

DaIconModes::DeleteDialogue::~DeleteDialogue(){}

/**********************/

void DaIconModes::proto_icon::copy() {
  CopyDialogue * cp =  new CopyDialogue( path + FileName , *parent->parent);
  cp->show();
  }

/**********************/

void DaIconModes::proto_icon::move() {
  MoveDialogue * mv =  new MoveDialogue( path + FileName , *parent->parent);
  mv->show();
  }

/**********************/

void DaIconModes::proto_icon::link() {
  LinkDialogue * ln =  new LinkDialogue( path + FileName , *parent->parent);
  ln->show();
  }

/**********************/

void DaIconModes::proto_icon::unlinkify() {
  DeleteDialogue * del =  new DeleteDialogue( path + FileName , *parent->parent);
  del->show();
  }

/**********************/
