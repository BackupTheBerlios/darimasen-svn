/* Darimasen - iconmodes.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

#include "iconmodes.h"
#include <iostream>

/**********************/

DaIconModes::DaIconModes(Glib::ustring  path) {                        

  filesAtPath = 0;
  try{
    Gnome::Vfs::DirectoryHandle handle;
    handle.open(path, Gnome::Vfs::FILE_INFO_DEFAULT);
    bool file_exists = true;
    while(file_exists){
      handle.read_next(file_exists);
      filesAtPath++;
      }
    }
  catch(const Gnome::Vfs::exception&){std::cout << "Miscount?\n";}



  fullPath = path;

  iconmode = 0;
  showHidden = 0;
  slotsUsed = 0;
  IconsHigh = 0;
  set_visible_window(false);
  hidden = new int[filesAtPath];

  if(iconmode == 0){
    sideconContainer = new Gtk::EventBox*[filesAtPath];
    for(int c = 0; c < filesAtPath; c++){
      sideconContainer[c] = new Gtk::EventBox;
      }

    try {
        // Call on_visit() for each file.
        // The options specify that we want to visit the files at input_uri_string,
        // get the mime type the fast way and protect against loops.
      Gnome::Vfs::DirectoryHandle::visit(fullPath, Gnome::Vfs::FILE_INFO_GET_MIME_TYPE |
                                             Gnome::Vfs::FILE_INFO_FORCE_FAST_MIME_TYPE |
                                             Gnome::Vfs::FILE_INFO_FOLLOW_LINKS ,
                                             Gnome::Vfs::DIRECTORY_VISIT_LOOPCHECK,
                                             sigc::mem_fun(*this,&DaIconModes::addEntry));
      }
    catch(const Gnome::Vfs::exception& ex){/*this is required.*/}
    }      



  }

/**********************/

DaIconModes::~DaIconModes(){

  // cleanup on aisles 1, 2, 3 ... and so on.
  if(iconmode == 0){
  for(int c = 0; c < filesAtPath; c++){
    Gtk::Widget * tmp = sideconContainer[c]->get_child();
    if (tmp)
      delete tmp;
    }

  for(int c = 0; c < filesAtPath; c++){
    delete sideconContainer[c];
    }

  delete sideconContainer;
}
}
/**********************/

bool DaIconModes::addEntry(
    const Glib::ustring& rel_path,
    const Glib::RefPtr<const Gnome::Vfs::FileInfo>& info,
    bool recursing_will_loop,
    bool& recurse) {

  if (info->get_type() != Gnome::Vfs::FILE_TYPE_DIRECTORY){
      sideconContainer[slotsUsed]->set_visible_window(false);
      sideconContainer[slotsUsed]->show();

hidden[slotsUsed] = (info->get_name().substr(0,1) == ".");

//std::cout << (info->get_name().substr(0,1) == ".") << " " << info->get_name() << "\n";

switch(iconmode){
case 0: {
Sidecon * tempPathS = new Sidecon(fullPath, info);
      tempPathS->show();
      sideconContainer[slotsUsed]->add(*tempPathS);
	}
break;
case 1: {
//Detail * tempPathD = new Detail(fullPath, info); 
//      tempPathD->show();
//     sideconContainer[slotsUsed]->add(*tempPathD);
	}
break;
}


      sideconContainer[slotsUsed]->set_events(Gdk::BUTTON_RELEASE_MASK);
      sideconContainer[slotsUsed]->signal_button_press_event().connect(
        sigc::bind<Glib::ustring >(
          sigc::mem_fun(*this, &DaIconModes::on_eventbox_button_press), info->get_name() ));
      slotsUsed++;
    }
  recurse = false; 
  return true;
  }

/**********************/

DaIconModes::Sidecon::Sidecon(
    Glib::ustring path,
    const Glib::RefPtr<const Gnome::Vfs::FileInfo>& info){
  filePath = path + info->get_name();

  Glib::ustring shortnom = info->get_name();

  if (info->get_name().length() > 25){

    int last = info->get_name().rfind(".");

    if (last != -1){ //fix for the missing extension bug
      Glib::ustring ext = info->get_name().substr(last);  
      shortnom = info->get_name().substr(0, 20) + ".." + ext;
      }
    else {
      shortnom = info->get_name().substr(0,22) + "...";
      }

    }
                   
  resize(3,2);


Glib::ustring ico;



ico = "/usr/share/icons/gnome/48x48/mimetypes/gnome-mime-";
ico += info->get_mime_type().replace(info->get_mime_type().find("/"), 1, "-");
ico += ".png";

/*

  try {

/*if(info->get_mime_type() == "image/jpeg"
    || info->get_mime_type() == "image/gif"
    || info->get_mime_type() == "image/png"){

  ico = info->get_name();
  }
else{
  ico = "/usr/share/icons/Lila/scalable/mimetypes/mime-";
  ico += info->get_mime_type().replace(info->get_mime_type().find("/"), 1, ":");
  ico += ".svg";
 // }

    Glib::file_get_contents(ico);
    }
  catch(const Glib::Error) {
ico = "/usr/share/icons/Lila/scalable/mimetypes/mime-";
ico += info->get_mime_type().substr(0,info->get_mime_type().find("/"));
ico += ".svg";
    }





Glib::RefPtr<Gdk::Pixbuf> xe = Gdk::Pixbuf::create_from_file(ico);
Glib::RefPtr<Gdk::Pixbuf> xf = xe->scale_simple(48,48,Gdk::INTERP_TILES);


  Gtk::Image * image1 = Gtk::manage(new class Gtk::Image(xf));
*/
  Gtk::Image * image1 = Gtk::manage(new class Gtk::Image(ico));


  Gtk::Label * FileName = Gtk::manage(new class Gtk::Label(shortnom));

  // this is where the mimetype info goes
  mimeInfo = info->get_mime_type();
  Gtk::Label * FilePermissions = Gtk::manage( new class Gtk::Label(mimeInfo));

  // THis is simply file size - images an video could have more. Much later.
  Glib::ustring size;
  if( info->get_size() < 1024 ){
    size = int2ustr(info->get_size()) + " B";
    }
  else if ( info->get_size() < (1024 * 1024) ){
    size = int2ustr(info->get_size() / 1024) + " KB";
    }
  else{
    size = int2ustr(info->get_size() / 1024 / 1024) + " MB";
    }

  // ideally, this label would be 80% size of standard
  Gtk::Label * FileSizeInfo = Gtk::manage( new class Gtk::Label(size));

  FileName->set_alignment(0,0.5);
  FileName->set_justify(Gtk::JUSTIFY_LEFT);
  FilePermissions->set_alignment(0,0.5);
  FilePermissions->set_justify(Gtk::JUSTIFY_LEFT);
  FileSizeInfo->set_alignment(0,0.5);
  FileSizeInfo->set_justify(Gtk::JUSTIFY_LEFT);
  
  attach(*image1, 0, 1, 0, 3, Gtk::FILL, Gtk::FILL, 0, 0);
  attach(*FileName, 1, 2, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
  attach(*FilePermissions, 1, 2, 1, 2, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
  attach(*FileSizeInfo, 1, 2, 2, 3, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
  
  image1->show();
  FileName->show();
  FilePermissions->show();
  FileSizeInfo->show();
  show();  }
/**********************

DaIconModes::Detail::Detail(
    Glib::ustring path,
    const Glib::RefPtr<const Gnome::Vfs::FileInfo>& info){


  filePath = path + info->get_name();

  Glib::ustring shortnom = info->get_name();

  if (info->get_name().length() > 25){

    int last = info->get_name().rfind(".");

    if (last != -1){ //fix for the missing extension bug
      Glib::ustring ext = info->get_name().substr(last);  
      shortnom = info->get_name().substr(0, 20) + ".." + ext;
      }
    else {
      shortnom = info->get_name().substr(0,22) + "...";
      }

    }
                   
  resize(1,3);
  //Gtk::Image * image1 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-dnd"), Gtk::IconSize(6)));
  Gtk::Label * FileName = Gtk::manage(new class Gtk::Label(shortnom));

  // this is where the mimetype info goes
  mimeInfo = info->get_mime_type();
  Gtk::Label * FilePermissions = Gtk::manage( new class Gtk::Label(mimeInfo));

  // THis is simply file size - images an video could have more. Much later.
  Glib::ustring size;
  if( info->get_size() < 1024 ){
    size = int2ustr(info->get_size()) + " B";
    }
  else if ( info->get_size() < (1024 * 1024) ){
    size = int2ustr(info->get_size() / 1024) + " KB";
    }
  else{
    size = int2ustr(info->get_size() / 1024 / 1024) + " MB";  std::cout << "2\n";
    }

  // ideally, this label would be 80% size of standard
  Gtk::Label * FileSizeInfo = Gtk::manage( new class Gtk::Label(size));

  FileName->set_alignment(0,0.5);
  FileName->set_justify(Gtk::JUSTIFY_LEFT);
  FilePermissions->set_alignment(0,0.5);
  FilePermissions->set_justify(Gtk::JUSTIFY_LEFT);
  FileSizeInfo->set_alignment(0,0.5);
  FileSizeInfo->set_justify(Gtk::JUSTIFY_LEFT);
  
  //attach(*image1, 0, 1, 0, 3, Gtk::FILL, Gtk::FILL, 0, 0);
  attach(*FileName, 0, 1, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
  attach(*FilePermissions, 1, 2, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
  attach(*FileSizeInfo, 2, 3, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
  
  //image1->show();
  FileName->show();
  FilePermissions->show();
  FileSizeInfo->show();
  show();  }

/**********************/


void DaIconModes::RunFile(const Glib::ustring file) {

  // see if the file is executable 

  Gnome::Vfs::Handle read_handle;
  Gnome::Vfs::Handle exec_handle;
  Glib::RefPtr<const Gnome::Vfs::FileInfo> info;
  
  try{
    read_handle.open(fullPath + file, Gnome::Vfs::OPEN_READ);
    info = read_handle.get_file_info(
      Gnome::Vfs::FILE_INFO_GET_MIME_TYPE |
      Gnome::Vfs::FILE_INFO_FORCE_SLOW_MIME_TYPE );
    }
  catch(const Gnome::Vfs::exception& ex){
    std::cout << "Does not exist.\n";
    return;
    }


  Glib::ustring exec = getenv("HOME");
  try{
    exec += "/Choices/MIME-types/";
    exec += info->get_mime_type().replace(info->get_mime_type().find("/"),1,"_");
    exec_handle.open(exec, Gnome::Vfs::OPEN_READ);

    exec += " \""  + fullPath + file + "\"";
    Glib::spawn_command_line_async(exec);

    std::cout << exec << " was run.\n";
    return;

    }
  catch(const Gnome::Vfs::exception& ex){
    std::cout << "Specific Mime Handler does not exist.\n";
    }


  try{
    exec = exec.substr(0, exec.rfind("_"));
    exec_handle.open(exec, Gnome::Vfs::OPEN_READ);
    exec += " \""  + fullPath + file + "\"";
    Glib::spawn_command_line_async(exec);

    std::cout << exec << " was run.\n";
    return;
    }
  catch(const Gnome::Vfs::exception& ex){
    std::cout << "Generic Mime Handler does not exist.\n";
    }

  if (Gnome::Vfs::Mime::can_be_executable(info->get_mime_type())){
    std::cout << " Executable - in fact, I'm running it.\n";
    Glib::spawn_command_line_async(fullPath + file);
    return;
    }
  else {
    std::cout << "Not identified as an executable either.\n";
    }
    

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
      exec += "/Choices/MIME-types/text_plain";
      exec += " \""  + fullPath + file + "\"";
      Glib::spawn_command_line_async(exec);
      std::cout << exec << " was run.\n";
      return;
      break;
    }
    default:
    {
      std::cout << "Well, that was usefull." << std::endl;
      break;
    }
  }

}



/**********************/

void DaIconModes::SetRunAction(const Glib::ustring file) {

  Gnome::Vfs::Handle read_handle;
  Glib::RefPtr<const Gnome::Vfs::FileInfo> info;
  Glib::ustring exec_subtype;
  Glib::ustring exec_mimetype;

  try {
    read_handle.open(fullPath + file, Gnome::Vfs::OPEN_READ);
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
    std::cout << "Err... Setting error?\n";
    }
  }

/**********************/

// make a generic icon action
bool DaIconModes::on_eventbox_button_press(GdkEventButton* event, const Glib::ustring Icon){
  if ((event->type == GDK_2BUTTON_PRESS) && (event->button == 1)){
     RunFile(Icon);
     static int x = 0;
     return true;
    }

  if ((event->type == GDK_BUTTON_PRESS) && (event->button == 3)){
    Gtk::Menu::MenuList& menulist = m_Menu_Popup.items();


    for( int i=9; i > 0; i--)
      m_Menu_Popup.items().pop_back();


        int startAtPos = 0;
    // following is needed so underscores show correctly
    menulist.push_back( Gtk::Menu_Helpers::MenuElem("Open \"" + underscoreSafe(Icon) + "\"",
      sigc::bind<Glib::ustring >(
        sigc::mem_fun(*this, &DaIconModes::RunFile),Icon)));

    menulist.push_back( Gtk::Menu_Helpers::MenuElem("Set Run Action...",
      sigc::bind<Glib::ustring>(
        sigc::mem_fun(*this, &DaIconModes::SetRunAction), Icon  )));

    menulist.push_back( Gtk::Menu_Helpers::MenuElem("Permissions...",
      sigc::bind<Glib::ustring>(
        sigc::mem_fun(*this, &DaIconModes::SetPermissions), Icon  )));

    menulist.push_back( Gtk::Menu_Helpers::SeparatorElem());


// this stuff needs tab completion to be usefull.
    //menulist.push_back( Gtk::Menu_Helpers::MenuElem("Move To..."));
    //menulist.push_back( Gtk::Menu_Helpers::MenuElem("Copy To..."));

    //menulist.push_back( Gtk::Menu_Helpers::MenuElem("Delete "));
    //menulist.push_back( Gtk::Menu_Helpers::MenuElem("Link"));
    //menulist.push_back( Gtk::Menu_Helpers::SeparatorElem());
    //menulist.push_back( Gtk::Menu_Helpers::MenuElem("Properties... "));

    m_Menu_Popup.popup(event->button, event->time);
    return true;
    }

  return false;
  }

/**********************/

void DaIconModes::on_size_allocate(Gtk::Allocation& allocation){
  Gtk::Widget * tmp = get_child();

  if(iconmode == 0){
    int oldie = IconsHigh;
    IconsHigh = allocation.get_height() / 60;
  
    if ( oldie != IconsHigh ){ // if resize is needed
      if (tmp)
        delete tmp; // actually DisposableTable, but segfaulted otherwise
      Gtk::Table * DisposableTable = new Gtk::Table((filesAtPath)/IconsHigh+1,IconsHigh);
      add(*DisposableTable);
      int y_pos = 0;
      int x_pos = 0;

      for(int i = 0; i < slotsUsed; i++){
        if( !hidden[i] || showHidden){
          DisposableTable->attach( *sideconContainer[i],
              x_pos, x_pos+1, y_pos, y_pos+1,
              Gtk::FILL, Gtk::FILL, 4, 4);
          sideconContainer[i]->show();
          y_pos++; 
          if(iconmode == 0){
            if ( y_pos + 1 > IconsHigh){
              y_pos = 0;
              x_pos++;
              }
            }
          }
        }
      DisposableTable->show();
      }
    }


  if(iconmode == 1 && !(tmp)){
    IconsHigh = 0;
    if (tmp)
      delete tmp; // actually DisposableTable, but segfaulted otherwise
    Gtk::Label * x = new Gtk::Label("Details go here.");
    x->show();
    add(*x);
    }
  Gtk::EventBox::on_size_allocate(allocation);
  }

/**********************/

DaIconModes::ChooseActionDialogue::ChooseActionDialogue(Glib::ustring mimeType){

  mime = mimeType;
  cancelbutton1 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-close")));
  okbutton1     = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-apply")));
  label1 = Gtk::manage(new class Gtk::Label("Enter a Shell command:"));
  entry1 = Gtk::manage(new class Gtk::Entry());
  vbox1  = Gtk::manage(new class Gtk::VBox(false, 0));


  radiobutton1 = Gtk::manage(
      new class Gtk::RadioButton(_RadioBGroup_radiobutton1, 
      "Set Mime For \"" + mimeType.substr(0, mimeType.find("_")) + "\"") );

  radiobutton1->signal_clicked().connect(
      sigc::bind<Glib::ustring >( sigc::mem_fun(*this,
      &DaIconModes::ChooseActionDialogue::GetCurrentAction), mimeType.substr(0, mimeType.find("_")) ));

 radiobutton2 = Gtk::manage(
      new class Gtk::RadioButton(_RadioBGroup_radiobutton1,
      "Set Mime For \"" + mimeType + "\"") ) ;
https://www.fusemail.com/sq/src/webmail.php
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
  cancelbutton1->show();
  okbutton1->show();
  radiobutton1->show();
  radiobutton2->show();
  label1->show();
  entry1->show();
  vbox1->show();
  show();
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
    std::cout << "The frellin mime file doesn't exist. Use an empty one.\n";
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
      std::cout << "New action set.\n";
      }
    catch(const Gnome::Vfs::exception) {
      std::cout << "couldn't write new definition.\n";
      hide();
      return;
      }
    }
  hide();
  return;
  }

/**********************/

void DaIconModes::SetPermissions(const Glib::ustring file) {


  Gnome::Vfs::Handle read_handle;
  Glib::RefPtr<Gnome::Vfs::FileInfo> info;

  try {
    read_handle.open(fullPath + file, Gnome::Vfs::OPEN_READ);
    info = read_handle.get_file_info(Gnome::Vfs::FILE_INFO_GET_ACCESS_RIGHTS);

    SetPermissionsDialogue * setPermissions;
    setPermissions = new SetPermissionsDialogue(info,fullPath);
    setPermissions->show();
    }
  catch(const Gnome::Vfs::exception& ex) {
    std::cout << "Err... Setting error?\n";
    }
  }

/**********************/

DaIconModes::SetPermissionsDialogue::SetPermissionsDialogue(
    Glib::RefPtr<Gnome::Vfs::FileInfo> info, Glib::ustring path){
fullPath = path;
  set_title("Set file permissions");
  set_modal(true);

   read = Gtk::manage(new class Gtk::Label("R"));
   write = Gtk::manage(new class Gtk::Label("W"));
   run = Gtk::manage(new class Gtk::Label("X"));
   user = Gtk::manage(new class Gtk::Label("User"));
   group = Gtk::manage(new class Gtk::Label("Group"));
   others = Gtk::manage(new class Gtk::Label("Everybody"));

   explaination = Gtk::manage(new class Gtk::Label("for \""+ info->get_name() + "\"..."));

   layout = Gtk::manage(new class Gtk::Table(5, 6, false));

   u_r = Gtk::manage(new class Gtk::CheckButton(""));
   if((info->get_permissions() & Gnome::Vfs::PERM_USER_READ) != 0)  u_r->set_active(true);
   u_w = Gtk::manage(new class Gtk::CheckButton(""));
   if((info->get_permissions() & Gnome::Vfs::PERM_USER_WRITE) != 0)  u_w->set_active(true);
   u_x = Gtk::manage(new class Gtk::CheckButton(""));
   if((info->get_permissions() & Gnome::Vfs::PERM_USER_EXEC) != 0)  u_x->set_active(true);
   g_r = Gtk::manage(new class Gtk::CheckButton(""));
   if((info->get_permissions() & Gnome::Vfs::PERM_GROUP_READ) != 0)  g_r->set_active(true);
   g_w = Gtk::manage(new class Gtk::CheckButton(""));
   if((info->get_permissions() & Gnome::Vfs::PERM_GROUP_WRITE) != 0)  g_w->set_active(true);
   g_x = Gtk::manage(new class Gtk::CheckButton(""));
   if((info->get_permissions() & Gnome::Vfs::PERM_GROUP_EXEC) != 0)  g_x->set_active(true);
   o_r = Gtk::manage(new class Gtk::CheckButton(""));
   if((info->get_permissions() & Gnome::Vfs::PERM_OTHER_READ) != 0)  o_r->set_active(true);
   o_w = Gtk::manage(new class Gtk::CheckButton(""));
   if((info->get_permissions() & Gnome::Vfs::PERM_OTHER_WRITE) != 0)  o_w->set_active(true);
   o_x = Gtk::manage(new class Gtk::CheckButton(""));
   if((info->get_permissions() & Gnome::Vfs::PERM_OTHER_EXEC) != 0)  o_x->set_active(true);

   extra = Gtk::manage(new  Gtk::VSeparator);

   sticky = Gtk::manage(new class Gtk::CheckButton("Sticky"));
   if((info->get_permissions() & Gnome::Vfs::PERM_STICKY) != 0)  sticky->set_active(true);
   GID = Gtk::manage(new class Gtk::CheckButton("Group ID"));
   if((info->get_permissions() & Gnome::Vfs::PERM_SGID) != 0)  GID->set_active(true);
   UID = Gtk::manage(new class Gtk::CheckButton("User ID"));;
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
  button1 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
  button1->signal_clicked().connect(
      sigc::mem_fun(*this, &DaIconModes::SetPermissionsDialogue::cancled) );


  button2 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-apply")));
  button2->signal_clicked().connect(
    sigc::bind<Glib::RefPtr<Gnome::Vfs::FileInfo> >(
      sigc::mem_fun(*this, &DaIconModes::SetPermissionsDialogue::apply), info ));


  get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
  add_action_widget(*button1, -6);
  add_action_widget(*button2, -10);

  button1->show();
  button2->show();

  set_modal(true);
  show_all_children();
  show();
  }

/**********************/

void DaIconModes::SetPermissionsDialogue::cancled(){hide();}

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
