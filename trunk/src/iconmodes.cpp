/* Darimasen - iconmodes.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

#include "iconmodes.h"
#include <iostream>


/**********************/

DaIconModes::DaIconModes(
    Glib::ustring  path,
    int            atPath,
    unsigned short usingMode,
  //  int            MainScrollerHeight,
    bool           givehidden ) {                        
  //There really should be a more efficient way of doing this...
  //resize(atPath,atPath);
  //double_buffered(); 
  fullPath = path;
  filesAtPath = atPath;
  iconmode = usingMode;
  //heightAvailable = MainScrollerHeight;
  showHidden = givehidden;
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
Detail * tempPathD = new Detail(fullPath, info); 
      tempPathD->show();
      sideconContainer[slotsUsed]->add(*tempPathD);
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

/**********************

bool DaIconModes::addDetail(
    const Glib::ustring& rel_path,
    const Glib::RefPtr<const Gnome::Vfs::FileInfo>& info,
    bool recursing_will_loop,
    bool& recurse) {

  if (info->get_type() != Gnome::Vfs::FILE_TYPE_DIRECTORY){
      sideconContainer[slotsUsed]->set_visible_window(false);
      sideconContainer[slotsUsed]->show();
      Detail * tempPath = new Detail(fullPath, info);
      tempPath->show();
      sideconContainer[slotsUsed]->add(*tempPath);
      sideconContainer[slotsUsed]->set_events(Gdk::BUTTON_RELEASE_MASK);
      sideconContainer[slotsUsed]->signal_button_press_event().connect(
        sigc::bind<Glib::ustring >(
          sigc::mem_fun(*this, &DaIconModes::on_eventbox_button_press), info->get_name() ));
      slotsUsed++;
    }
  recurse = false; 
  return true;
  }

/**********************


bool DaIconModes::addDetail(
    const Glib::ustring& rel_path,
    const Glib::RefPtr<const Gnome::Vfs::FileInfo>& info,
    bool recursing_will_loop,
    bool& recurse) {


  if (info->get_type() != Gnome::Vfs::FILE_TYPE_DIRECTORY){
    //if ( (info->get_name().substr(0,1) != ".") || showHidden ) {

      sideconContainer[slotsUsed]->set_visible_window(false);
      sideconContainer[slotsUsed]->show();

      Gtk::Label * tempPath = new Gtk::Label(info->get_name());
std::cout << info->get_name() << "\n";
      //tempPath->set_alignment(0,0.5);
      tempPath->show();

      sideconContainer[slotsUsed]->add(*tempPath);
      sideconContainer[slotsUsed]->set_events(Gdk::BUTTON_RELEASE_MASK);
      sideconContainer[slotsUsed]->signal_button_press_event().connect(
        sigc::bind<Glib::ustring >(
          sigc::mem_fun(*this, &DaIconModes::on_eventbox_button_press), info->get_name() ));

       slotsUsed++;
      //}
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

   /* XXX: These should all be initalized to NULL to keep our arse out of the */
   /* proverbial fire. Mmmm proverbial fire *drool*                           */
   /* Widgets *************************************************************** */
   Gtk::RadioButton::Group _RadioBGroup_radiobutton1;
   Gtk::Dialog      * dialog1;
   Gtk::Button      * cancelbutton1;
   Gtk::Button      * okbutton1;
   Gtk::RadioButton * radiobutton1;
   Gtk::RadioButton * radiobutton2;
   Gtk::Label       * label1;
   Gtk::Entry       * entry1;
   Gtk::VBox        * vbox1;
   /* *********************************************************************** */

   dialog1       = new Gtk::Dialog;
   cancelbutton1 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-close")));
   okbutton1     = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-apply")));
   label1 = Gtk::manage(new class Gtk::Label("Enter a Shell command:"));
   entry1 = Gtk::manage(new class Gtk::Entry());
   vbox1  = Gtk::manage(new class Gtk::VBox(false, 0));
  
	try {

	read_handle.open(fullPath + file, Gnome::Vfs::OPEN_READ);

	info = read_handle.get_file_info(
			Gnome::Vfs::FILE_INFO_GET_MIME_TYPE |
			Gnome::Vfs::FILE_INFO_FORCE_SLOW_MIME_TYPE );


	exec_subtype  = info->get_mime_type();
	exec_subtype  = exec_subtype.replace( exec_subtype.find("/"), 1, "_" );
	exec_mimetype = exec_subtype.substr( 0, exec_subtype.rfind("_") );

	radiobutton1 = Gtk::manage(
		new class Gtk::RadioButton(_RadioBGroup_radiobutton1, 
			"Set Mime For \"" + exec_mimetype + "\"") );

	radiobutton2 = Gtk::manage(
		new class Gtk::RadioButton(_RadioBGroup_radiobutton1,
			"Set Mime For \"" + info->get_mime_type() + "\"") ) ;

	cancelbutton1->set_flags(Gtk::CAN_FOCUS);
	cancelbutton1->set_flags(Gtk::CAN_DEFAULT);
	cancelbutton1->set_relief(Gtk::RELIEF_NORMAL);
	okbutton1->set_flags(Gtk::CAN_FOCUS);
	okbutton1->set_flags(Gtk::CAN_DEFAULT);
	okbutton1->set_relief(Gtk::RELIEF_NORMAL);
	dialog1->get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
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
	dialog1->get_vbox()->set_border_width(6);
	dialog1->get_vbox()->pack_start(*vbox1);
	dialog1->set_border_width(6);
	dialog1->set_title("Set Run Action");
	dialog1->set_modal(false);
	dialog1->property_window_position().set_value(Gtk::WIN_POS_CENTER_ON_PARENT);
	dialog1->set_resizable(true);
	dialog1->property_destroy_with_parent().set_value(false);
	dialog1->set_has_separator(false);
	dialog1->add_action_widget(*cancelbutton1, Gtk::RESPONSE_CANCEL);
	dialog1->add_action_widget(*okbutton1, Gtk::RESPONSE_OK);
	dialog1->set_default_response(Gtk::RESPONSE_OK);
	cancelbutton1->show();
	okbutton1->show();
	radiobutton1->show();
	radiobutton2->show();
	label1->show();
	entry1->show();
	vbox1->show();

	/* XXX: Check for mimetype handler. Currently only looks for sub */
	Glib::ustring contents, exec1 = getenv("HOME");
	exec1 += (Glib::ustring)("/Choices/MIME-types/");
	exec1 += exec_subtype;

	try {
		contents = Glib::file_get_contents(exec1);
		contents = contents.substr(contents.find("exec ") + 5);
		contents = contents.substr(0,contents.find("\n"));
	} catch(const Glib::Error) {
		std::cout << "The frellin mime file doesn't exist. Use an empty one.\n";
		contents = "* \"$@\"" ;
	}

	entry1->set_text(contents);

	if (dialog1->run() ==  Gtk::RESPONSE_OK) {
		std::cout << "In here!" << std::endl;
		Gnome::Vfs::Handle write_handle;

		if(entry1->get_text().substr(0,1) == "*") {
			std::cout << "Nothing was changed! Rejecting.\n";
			dialog1->hide();
			return;
		}

		Glib::ustring contents, exec1 = getenv("HOME");
		exec1 += (Glib::ustring)("/Choices/MIME-types/");
		exec1 += (radiobutton1->get_active()) ? exec_mimetype : exec_subtype;

		Glib::ustring command = "#! /bin/sh\n#Made by Darimasen\nexec ";
		command += entry1->get_text();
		command += "\n";
		std::cout << "New script is:\n" << command;

		try {
			// Now write the data we read out to the output uri
			write_handle.create(exec1, Gnome::Vfs::OPEN_WRITE, false, 0755);
			write_handle.seek(Gnome::Vfs::SEEK_POS_START, 0);
			GnomeVFSFileSize bytes_written = write_handle.write(command.data(), command.size());
			
		} catch(const Gnome::Vfs::exception) {
					std::cout << "couldn't write new definition.\n";
					dialog1->hide();
					return;
		} // try for file write

	}

	dialog1->hide();

	} catch(const Gnome::Vfs::exception& ex) {
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

void DaIconModes::on_size_allocate(Gtk::Allocation& allocation){

Gtk::Widget * tmp = get_child();

  if(iconmode == 0){
    int oldie = IconsHigh;
    IconsHigh = allocation.get_height() / 58;
  
    if ( (oldie != IconsHigh) ){ // if resize is needed
      if (tmp)
        delete tmp; // actually DisposableTable, but segfaulted otherwise
      Gtk::Table * DisposableTable = new Gtk::Table((filesAtPath)/IconsHigh+1,IconsHigh);
      add(*DisposableTable);
      int y_pos = 0;
      int x_pos = 0;

// std::cout << slotsUsed << " " << filesAtPath << "\n";
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
        }}
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
