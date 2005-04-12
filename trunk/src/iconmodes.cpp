/* Darimasen - iconmodes.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

#include "iconmodes.h"
#include <iostream>

/**********************/

DaIconModes::DaIconModes(guint pos, Darimasen& myParent) {    

  parent = &myParent;

position = pos;

lastclick = 0;
  filesAtPath = 0;
  try{
    Gnome::Vfs::DirectoryHandle handle;
    handle.open((parent->history[position].top()), Gnome::Vfs::FILE_INFO_DEFAULT);
    bool file_exists = true;
    while(file_exists){
      handle.read_next(file_exists);
      filesAtPath++;
      }
    }
  catch(const Gnome::Vfs::exception&){std::cout << "Miscount?\n";}


 // iconmode = 0;

  slotsUsed = 0;
  IconsHigh = 0;
  set_visible_window(false);
  hidden = new int[filesAtPath];

  if(parent->mode == 0){
    sideconContainer = new Gtk::EventBox*[filesAtPath];
    for(int c = 0; c < filesAtPath; c++){
      sideconContainer[c] = new Gtk::EventBox;
      }

    try {
        // Call on_visit() for each file.
        // The options specify that we want to visit the files at input_uri_string,
        // get the mime type the fast way and protect against loops.
      Gnome::Vfs::DirectoryHandle::visit( parent->history[position].top(), Gnome::Vfs::FILE_INFO_GET_MIME_TYPE |
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
// there is an issue that this could be deleting while in the wrong mode - yeah.
  if(parent->mode == 0 ){
    for(int c = 0; c < filesAtPath; c++){
      Gtk::Widget * tmp = sideconContainer[c]->get_child();
      if (tmp)
        delete tmp;
        }

    for(int c = 0; c < filesAtPath; c++){
      delete sideconContainer[c];
      }
    delete sideconContainer;
    delete hidden;
  }
}
/**********************/

bool DaIconModes::addEntry(
    Glib::ustring rel_path,
    Glib::RefPtr<const Gnome::Vfs::FileInfo> info,
    bool recursing_will_loop,
    bool recurse) {

  if (info->get_type() != Gnome::Vfs::FILE_TYPE_DIRECTORY){
      sideconContainer[slotsUsed]->set_visible_window(false);
      sideconContainer[slotsUsed]->show();

hidden[slotsUsed] = (info->get_name().substr(0,1) == ".");

switch(parent->mode){
case 0: {
Sidecon * tempPathS = new Sidecon( parent->history[position].top(), info, *this);
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
      const Glib::RefPtr<const Gnome::Vfs::FileInfo>& info,
      DaIconModes& above){

  parent = &above;
  filePath = path + info->get_name();

  if (info->get_name().length() > 25){
    int last = info->get_name().rfind(".");
    if (last != -1){
      FileName = new class Gtk::Label( info->get_name().substr(0, 20) + ".." + info->get_name().substr(last) );
      }
    else {
      FileName = new class Gtk::Label( info->get_name().substr(0,22) + "..." );
      }
    }
  else {
  FileName = new class Gtk::Label( info->get_name() );
  }
                   


//  FileName = new class Gtk::Label(shortnom);
  FileName->set_alignment(0,0.5);
  FileName->set_justify(Gtk::JUSTIFY_LEFT);
  attach(*FileName, 1, 2, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);


  // hand off getting the icon
  image1 = new class Gtk::Image(parent->getIcon(info->get_mime_type(), 48));
  attach(*image1, 0, 1, 0, 3, Gtk::FILL, Gtk::FILL, 0, 0);


  // this is where the mimetype info goes
  FilePermissions = new class Gtk::Label(info->get_mime_type());
  FilePermissions->set_alignment(0,0.5);
  FilePermissions->set_justify(Gtk::JUSTIFY_LEFT);
  attach(*FilePermissions, 1, 2, 1, 2, Gtk::FILL, Gtk::AttachOptions(), 0, 0);


  // This is simply file size - images an video could have more. Later.
  if( info->get_size() < 1024 ){
    FileSizeInfo = new class Gtk::Label(int2ustr(info->get_size()) + " B");
    }
  else if ( info->get_size() < (1024 * 1024) ){
    FileSizeInfo = new class Gtk::Label(int2ustr(info->get_size() / 1024) + " KB");
    }
  else{
    FileSizeInfo = new class Gtk::Label( int2ustr(info->get_size() / 1048576) + " MB");
    }
  FileSizeInfo->set_alignment(0,0.5);
  FileSizeInfo->set_justify(Gtk::JUSTIFY_LEFT);
  attach(*FileSizeInfo, 1, 2, 2, 3, Gtk::FILL, Gtk::AttachOptions(), 0, 0);


  show_all_children();  }

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
    read_handle.open( parent->history[position].top() + file, Gnome::Vfs::OPEN_READ);
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

    exec += " \""  + parent->history[position].top() + file + "\"";
    Glib::spawn_command_line_async(exec);

    parent->set_message(exec + " was run.");
    return;

    }
  catch(const Gnome::Vfs::exception& ex){
    }


  try{
    exec = exec.substr(0, exec.rfind("_"));
    exec_handle.open(exec, Gnome::Vfs::OPEN_READ);
    exec += " \""  + parent->history[position].top() + file + "\"";
    Glib::spawn_command_line_async(exec);

    parent->set_message(exec + " was run.");
    return;
    }
  catch(const Gnome::Vfs::exception& ex){
    }

  if (Gnome::Vfs::Mime::can_be_executable(info->get_mime_type())){
    parent->set_message( "Running " + parent->history[position].top()  + file);
    Glib::spawn_command_line_async( parent->history[position].top()  + file);
    return;
    }
  else {
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
      exec += " \""  + parent->history[position].top()  + file + "\"";
      Glib::spawn_command_line_async(exec);
    parent->set_message(exec + " was opened as a text file.");
      return;
      break;
    }
    default:
    {
    parent->set_message("Well, that was usefull.");
      break;
    }
  }

}



/**********************/


void DaIconModes::RunFileAsText(const Glib::ustring file) {

  // see if the file is executable 

  Gnome::Vfs::Handle read_handle;
  Gnome::Vfs::Handle exec_handle;
  Glib::RefPtr<const Gnome::Vfs::FileInfo> info;
  
  try{
    read_handle.open( parent->history[position].top() + file, Gnome::Vfs::OPEN_READ);
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
    exec += "/Choices/MIME-types/text";
    exec_handle.open(exec, Gnome::Vfs::OPEN_READ);

    exec += " \""  + parent->history[position].top() + file + "\"";
    Glib::spawn_command_line_async(exec);

    parent->set_message(exec + " was run.");
    return;

    }
  catch(const Gnome::Vfs::exception& ex){
    }

}



/**********************/

void DaIconModes::SetRunAction(const Glib::ustring file) {

  Gnome::Vfs::Handle read_handle;
  Glib::RefPtr<const Gnome::Vfs::FileInfo> info;
  Glib::ustring exec_subtype;
  Glib::ustring exec_mimetype;

  try {
    read_handle.open( parent->history[position].top() + file, Gnome::Vfs::OPEN_READ);
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
    parent->set_message("Err... Setting error?");
    }
  }

/**********************/

// make a generic icon action
bool DaIconModes::on_eventbox_button_press(GdkEventButton* event, const Glib::ustring Icon){

  if ((event->type == GDK_BUTTON_PRESS) && (event->button == 1)  && (lastclick < event->time)){
    lastclick = event->time + 1000;
    RunFile(Icon);
 //   static int x;
    return true;
    }

  if ((event->type == GDK_BUTTON_PRESS) && (event->button == 3)){
    //Gtk::Menu::MenuList& menulist = m_Menu_Popup.items();


    for( int i=10; i > 0; i--)
      m_Menu_Popup.items().pop_back();
//std::cout << Icon << "\n";

        int startAtPos = 0;
    // following is needed so underscores show correctly
    m_Menu_Popup.items().push_back( Gtk::Menu_Helpers::MenuElem("Open \"" + underscoreSafe(Icon) + "\"",
      sigc::bind<Glib::ustring >(
        sigc::mem_fun(*this, &DaIconModes::RunFile),Icon)));


    m_Menu_Popup.items().push_back( Gtk::Menu_Helpers::MenuElem("Open as text ",
      sigc::bind<Glib::ustring >(
        sigc::mem_fun(*this, &DaIconModes::RunFileAsText),Icon)));


    m_Menu_Popup.items().push_back( Gtk::Menu_Helpers::SeparatorElem());


    m_Menu_Popup.items().push_back( Gtk::Menu_Helpers::MenuElem("Set Run Action...",
      sigc::bind<Glib::ustring>(
        sigc::mem_fun(*this, &DaIconModes::SetRunAction), Icon  )));

    m_Menu_Popup.items().push_back( Gtk::Menu_Helpers::MenuElem("Permissions...",
      sigc::bind<Glib::ustring>(
        sigc::mem_fun(*this, &DaIconModes::SetPermissions), Icon  )));

    m_Menu_Popup.items().push_back( Gtk::Menu_Helpers::SeparatorElem());

    m_Menu_Popup.items().push_back( Gtk::Menu_Helpers::MenuElem("Copy To...",
      sigc::bind<Glib::ustring>(
        sigc::mem_fun(*this, &DaIconModes::copy), Icon  )));



    m_Menu_Popup.items().push_back( Gtk::Menu_Helpers::MenuElem("Move To...",
      sigc::bind<Glib::ustring>(
        sigc::mem_fun(*this, &DaIconModes::move), Icon  )));

    m_Menu_Popup.items().push_back( Gtk::Menu_Helpers::MenuElem("Link To...",
      sigc::bind<Glib::ustring>(
        sigc::mem_fun(*this, &DaIconModes::link), Icon  )));

    m_Menu_Popup.items().push_back( Gtk::Menu_Helpers::MenuElem("Delete ",
      sigc::bind<Glib::ustring>(
        sigc::mem_fun(*this, &DaIconModes::unlinkify), Icon  )));


    m_Menu_Popup.popup(event->button, event->time);
    return true;
    }

  return false;
  }

/**********************/

void DaIconModes::on_size_allocate(Gtk::Allocation& allocation){


  if(parent->mode == 0){
    int oldie = IconsHigh;
    IconsHigh = allocation.get_height() / 58;
  
    if ( oldie != IconsHigh ){

redraw(); // if resize is needed
     
      }
    }


  if(parent->mode == 1 ){
   
    Gtk::Widget * tmp = get_child();
if (!tmp){
    IconsHigh = 0;
    if (tmp)
      delete tmp; // actually DisposableTable, but segfaulted otherwise
    Gtk::Label * x = new Gtk::Label("Details go here.");
    x->show();
    add(*x);
    }}
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
        if(  !hidden[i] || parent->optShowHidden->get_active()){
          DisposableTable->attach( *sideconContainer[i],
              x_pos, x_pos+1, y_pos, y_pos+1,
              Gtk::FILL, Gtk::FILL, 4, 4);
          sideconContainer[i]->show();
          y_pos++; 
          if(parent->mode == 0){
            if ( y_pos + 1 > IconsHigh){
              y_pos = 0;
              x_pos++;
              }
            }
          }
        }
      DisposableTable->show();
  
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

void DaIconModes::SetPermissions(const Glib::ustring file) {


  Gnome::Vfs::Handle read_handle;
  Glib::RefPtr<Gnome::Vfs::FileInfo> info;

  try {
    read_handle.open( parent->history[position].top() + file, Gnome::Vfs::OPEN_READ);
    info = read_handle.get_file_info(Gnome::Vfs::FILE_INFO_GET_ACCESS_RIGHTS);

    SetPermissionsDialogue * setPermissions;
    setPermissions = new SetPermissionsDialogue(info,parent->history[position].top());
    setPermissions->show();
    }
  catch(const Gnome::Vfs::exception& ex) {
    parent->set_message("Err... Setting error?");
    }
  }

/**********************/

void DaIconModes::copy(const Glib::ustring file) {
  CopyDialogue * cp =  new CopyDialogue( parent->history[position].top() + file , *parent);
  cp->show();
  }

/**********************/

void DaIconModes::move(const Glib::ustring file) {
  MoveDialogue * mv =  new MoveDialogue( parent->history[position].top() + file , *parent);
  mv->show();
  }

/**********************/

void DaIconModes::link(const Glib::ustring file) {
  LinkDialogue * ln =  new LinkDialogue( parent->history[position].top() + file , *parent);
  ln->show();
  }

/**********************/

void DaIconModes::unlinkify(const Glib::ustring file) {
  DeleteDialogue * del =  new DeleteDialogue( parent->history[position].top() + file , *parent);
  del->show();
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

Glib::RefPtr<Gdk::Pixbuf> DaIconModes::getIcon(Glib::ustring mimeGiven, guint size){
  int i;
  static std::vector <Glib::ustring> mimeList;
  static std::vector < Glib::RefPtr<Gdk::Pixbuf> > unsizedImg;


  for(i = 0; i< mimeList.size(); i++){
    if( mimeGiven == mimeList[i] ){
      Glib::RefPtr<Gdk::Pixbuf> scale = unsizedImg[i]->scale_simple(size,size,Gdk::INTERP_TILES);
      return scale;
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

  Glib::RefPtr<Gdk::Pixbuf> scale = unsizedImg[i]->scale_simple(size,size,Gdk::INTERP_TILES);
  return scale; 
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
//  std::cout << "copying " << label->get_text()
//    << "\nto " << entry->get_text() << "\n\n";

//rename(label->get_text().c_str(), entry->get_text().c_str());
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
//  std::cout << "moveing " << label->get_text()
//    << "\nto " << entry->get_text() << "\n\n";

 // rename(label->get_text().c_str(), entry->get_text().c_str());

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
//  std::cout << "linking " << label->get_text()
 //   << "\nto " << entry->get_text() << "\n\n";

  //symlink(label->get_text().c_str(), entry->get_text().c_str());
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
//  std::cout << "delete " << label->get_text() << "\n\n";

 // unlink( label->get_text().c_str() );
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
