/* Darimasen - iconmodes.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

#include "iconmodes.h"
#include <iostream>


/**********************/

DaIconModes::DaIconModes(
    Glib::ustring  path,
    int            atPath,
    unsigned short usingMode,
    int            MainScrollerHeight,
    bool           givehidden ) {
                                  
  resize(atPath,atPath);
  double_buffered();
    
  fullPath = path;
  filesAtPath = atPath;
  iconmode = usingMode;
  heightAvailable = MainScrollerHeight;
  showHidden = givehidden;

  x_pos = 0;
  y_pos = 0;

// 0 is sidecon view
// 1 is detail view
    
  
  switch(iconmode){  
    case 0:
      
      try {
        // Call on_visit() for each file.
        // The options specify that we want to visit the files at input_uri_string,
        // get the mime type the fast way and protect against loops.
        Gnome::Vfs::DirectoryHandle::visit(fullPath, Gnome::Vfs::FILE_INFO_GET_MIME_TYPE |
                                             Gnome::Vfs::FILE_INFO_FORCE_FAST_MIME_TYPE |

Gnome::Vfs::FILE_INFO_FOLLOW_LINKS ,
                                             Gnome::Vfs::DIRECTORY_VISIT_LOOPCHECK,
                                             sigc::mem_fun(*this,&DaIconModes::addSidecon));
        }
      catch(const Gnome::Vfs::exception& ex){/*this is required.*/}
      break;

    case 1:
      try {
        // Call on_visit() for each file.
        // The options specify that we want to visit the files at input_uri_string,
        // get the mime type the fast way and protect against loops.
        Gnome::Vfs::DirectoryHandle::visit(fullPath, Gnome::Vfs::FILE_INFO_GET_MIME_TYPE |
                                             Gnome::Vfs::FILE_INFO_FORCE_FAST_MIME_TYPE,
                                             Gnome::Vfs::DIRECTORY_VISIT_LOOPCHECK,
                                             sigc::mem_fun(*this,&DaIconModes::addDetail));
        }
      catch(const Gnome::Vfs::exception& ex){/*this is required.*/}
      break;
    } 
  }

/**********************/


bool DaIconModes::addSidecon(
    const Glib::ustring& rel_path,
    const Glib::RefPtr<const Gnome::Vfs::FileInfo>& info,
    bool recursing_will_loop,
    bool& recurse) {

  if (info->get_type() != Gnome::Vfs::FILE_TYPE_DIRECTORY){
    if ( (info->get_name().substr(0,1) != ".") || showHidden ) {

      Gtk::EventBox * sideconContainer = Gtk::manage(new Gtk::EventBox);
      sideconContainer->set_visible_window(false);
      sideconContainer->show();

      Sidecon * tempPath = new Sidecon(fullPath, info);
      tempPath->show();

      sideconContainer->add(*tempPath);

      attach(*sideconContainer, x_pos, x_pos+1, y_pos, y_pos+1, Gtk::FILL, Gtk::FILL, 4, 4);

      sideconContainer->set_events(Gdk::BUTTON_RELEASE_MASK);
      sideconContainer->signal_button_press_event().connect(
        sigc::bind<Glib::ustring >(
          sigc::mem_fun(*this, &DaIconModes::on_eventbox_button_press), info->get_name() ));

      y_pos++; // positioning stuff. 
      if ( y_pos + 1 > (heightAvailable / 58)){
        y_pos = 0;
        x_pos++;
        }
      }
    }
  

  recurse = false; // just this folder
  return true;
}


/**********************/


bool DaIconModes::addDetail(
    const Glib::ustring& rel_path,
    const Glib::RefPtr<const Gnome::Vfs::FileInfo>& info,
    bool recursing_will_loop,
    bool& recurse) {

  if (info->get_type() != Gnome::Vfs::FILE_TYPE_DIRECTORY){
    if ((info->get_name().substr(0,1) != ".") xor (showHidden == true)){
      Gtk::Label * tempPath = Gtk::manage(new Gtk::Label(info->get_name()));
      tempPath->set_alignment(0,0.5);
      attach(*tempPath, 0, 1, y_pos, y_pos+1, Gtk::FILL, Gtk::FILL, 4, 4);
      tempPath->show();
      y_pos++;
      }
    }
  recurse = false; // just this folder
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
  Gtk::Image * image1 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-dnd"), Gtk::IconSize(6)));
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
/**********************/

DaIconModes::Sidecon::~Sidecon(){
  }

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
  Glib::ustring exec;
  
  try{

    read_handle.open(fullPath + file, Gnome::Vfs::OPEN_READ);

    info = read_handle.get_file_info(
      Gnome::Vfs::FILE_INFO_GET_MIME_TYPE |
      Gnome::Vfs::FILE_INFO_FORCE_SLOW_MIME_TYPE );


    exec = info->get_mime_type().replace(info->get_mime_type().find("/"),1,"_");




   //Glade rip!



   Gtk::Dialog * dialog1 = new Gtk::Dialog;


   Gtk::Button * cancelbutton1 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
   Gtk::Button * okbutton1 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));

   
   Gtk::RadioButton::Group _RadioBGroup_radiobutton1;



   Gtk::RadioButton *radiobutton1 = Gtk::manage(
     new class Gtk::RadioButton(_RadioBGroup_radiobutton1,
       "Set Mime For all " + exec.substr(0, exec.rfind("_"))));
   Gtk::RadioButton *radiobutton2 = Gtk::manage(
     new class Gtk::RadioButton(_RadioBGroup_radiobutton1,
      "Set Mime For " + exec));
   Gtk::Label * label1 = Gtk::manage(new class Gtk::Label("Enter a Shell command:"));
   Gtk::Entry * entry1 = Gtk::manage(new class Gtk::Entry());
   Gtk::VBox * vbox1 = Gtk::manage(new class Gtk::VBox(false, 0));
   cancelbutton1->set_flags(Gtk::CAN_FOCUS);
   cancelbutton1->set_flags(Gtk::CAN_DEFAULT);
   cancelbutton1->set_relief(Gtk::RELIEF_NORMAL);
   okbutton1->set_flags(Gtk::CAN_FOCUS);
   okbutton1->set_flags(Gtk::CAN_DEFAULT);
   okbutton1->set_relief(Gtk::RELIEF_NORMAL);
   dialog1->get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
   radiobutton1->set_sensitive(false);
   radiobutton1->set_flags(Gtk::CAN_FOCUS);
   radiobutton1->set_relief(Gtk::RELIEF_NORMAL);
   radiobutton1->set_mode(true);
   radiobutton1->set_active(false);
   radiobutton2->set_flags(Gtk::CAN_FOCUS);
   radiobutton2->set_relief(Gtk::RELIEF_NORMAL);
   radiobutton2->set_mode(true);
   radiobutton2->set_active(true);
   label1->set_alignment(0,0.48);
   label1->set_padding(2,4);
   label1->set_justify(Gtk::JUSTIFY_LEFT);
   label1->set_line_wrap(false);
   label1->set_use_markup(false);
   label1->set_selectable(false);
   entry1->set_flags(Gtk::CAN_FOCUS);
   entry1->set_visibility(true);
   entry1->set_editable(true);
   entry1->set_max_length(0);

   entry1->set_has_frame(true);
   entry1->set_activates_default(false);
   vbox1->pack_start(*radiobutton1, Gtk::PACK_SHRINK, 0);
   vbox1->pack_start(*radiobutton2, Gtk::PACK_SHRINK, 0);
   vbox1->pack_start(*label1, Gtk::PACK_SHRINK, 0);
   vbox1->pack_start(*entry1, Gtk::PACK_SHRINK, 0);
   dialog1->get_vbox()->set_homogeneous(false);
   dialog1->get_vbox()->set_spacing(5);
   dialog1->get_vbox()->pack_start(*vbox1);
   dialog1->set_title("Set Run Action");
   dialog1->set_modal(false);
   dialog1->property_window_position().set_value(Gtk::WIN_POS_CENTER_ON_PARENT);
   dialog1->set_resizable(true);
   dialog1->property_destroy_with_parent().set_value(false);
   dialog1->set_has_separator(true);
   dialog1->add_action_widget(*cancelbutton1, -6);
   dialog1->add_action_widget(*okbutton1, -5);
   cancelbutton1->show();
   okbutton1->show();
   radiobutton1->show();
   radiobutton2->show();
   label1->show();
   entry1->show();
   vbox1->show();


   Glib::ustring contents, exec1 = getenv("HOME");
   exec1 += (Glib::ustring)("/Choices/MIME-types/");
   exec1 += exec;

   try{
     contents = Glib::file_get_contents(exec1);
     contents = contents.substr(contents.find("exec ") + 5);
     contents = contents.substr(0,contents.find("\n"));
     }
   catch(const Glib::Error){
     std::cout << "The frellin mime file doesn't exist. Use an empty one.\n";
     contents = "* \"$@\"" ;
     }
   entry1->set_text(contents);



  switch(dialog1->run())
  {
    case(Gtk::RESPONSE_OK):
    {
      Gnome::Vfs::Handle write_handle;

      try{

        if(entry1->get_text().substr(0,1) == "*"){
          std::cout << "Nothing was changed! Rejecting.\n";
          dialog1->hide();
          return;
          }


        write_handle.create(exec1, Gnome::Vfs::OPEN_WRITE, false, 0755);

        write_handle.seek(Gnome::Vfs::SEEK_POS_START, 0);
        Glib::ustring command = "#! /bin/sh\n#Made by Darimasen\nexec ";
        command += entry1->get_text();
        command += "\n";
        std::cout << "New script is:\n" << command;

        // Now write the data we read out to the output uri
        GnomeVFSFileSize bytes_written = write_handle.write(command.data(), command.size());

        }
      catch(const Gnome::Vfs::exception){
        std::cout << "couldn't write new definition.\n";
        dialog1->hide();
        return;
        }

      dialog1->hide();
      return;
      break;
    }
    default:
    {
      std::cout << "There is no change to the script" << std::endl;
      dialog1->hide();
      return;
      break;
    }
  }









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

    // following is needed so underscores show correctly
    Gtk::MenuItem * op = Gtk::manage( new Gtk::MenuItem("Open \"" + Icon + "\""));
    op->signal_activate().connect(
      sigc::bind<Glib::ustring >(
        sigc::mem_fun(*this, &DaIconModes::RunFile),Icon) );
    op->show();
    menulist.push_back( Gtk::Menu_Helpers::MenuElem(*op));


menulist.push_back( Gtk::Menu_Helpers::MenuElem("Set Run Action"
,
    sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &DaIconModes::SetRunAction), Icon  )));

    //menulist.push_back( Gtk::Menu_Helpers::MenuElem("Rename"));
    //menulist.push_back( Gtk::Menu_Helpers::MenuElem("Delete "));
    menulist.push_back( Gtk::Menu_Helpers::SeparatorElem());
    //menulist.push_back( Gtk::Menu_Helpers::MenuElem("Copy"));
    //menulist.push_back( Gtk::Menu_Helpers::MenuElem("Move"));
    //menulist.push_back( Gtk::Menu_Helpers::MenuElem("Link"));
    //menulist.push_back( Gtk::Menu_Helpers::SeparatorElem());
    //menulist.push_back( Gtk::Menu_Helpers::MenuElem("Properties... "));

    m_Menu_Popup.popup(event->button, event->time);
    return true;
    }

  return false;
  }

/**********************/
