/* Darimasen - iconmodes.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

#include "iconmodes.h"

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
filesAtPath++;
    Gtk::TreeModel::Row row = *(m_refTreeModel->append());

    row[iconlist->m_col_icon] = getIcon(info->get_mime_type());

    row[iconlist->m_col_name] = info->get_name();


	Glib::ustring sized;
        if( info->get_size() < 1024 ){
          sized = (int2ustr(info->get_size()) + " B");
          }
        else if ( info->get_size() < (1024 * 1024) ){
          sized = (int2ustr(info->get_size() / 1024) + " KB");
          }
        else{
          sized = (int2ustr(info->get_size() / 1048576) + " MB");
          }
          row[iconlist->m_col_size] = sized;


        row[iconlist->m_col_mime] = info->get_mime_type();



        row[iconlist->m_col_all] = (info->get_name().substr(0,25) + "\n" + sized + "\n" + info->get_mime_type().substr(0,25));


//  if (info->get_type() != Gnome::Vfs::FILE_TYPE_DIRECTORY){
//    iconlist[slotsUsed++] = new proto_icon(*this, parent->get_history(position), info);
    }

  return true;
  }

/**********************/

void DaIconModes::on_size_allocate(Gtk::Allocation& allocation){

 switch(mode){
   case 0:
     
       m_TreeView.set_columns((filesAtPath - 1)/(allocation.get_height() / 60) + 1);
  /*
    // std::vector<guint> * order = new std::vector<guint>;
     for(int i = 0; i < (allocation.get_height() / 60); i++){
       for(int j = 0; j < filesAtPath; j += (allocation.get_height() / 60)){
         std::cout<< i+j << "\n";//order->push_back(j+i);
	 }
       }

*/
     break;
   case 1:
     m_TreeView.set_columns(filesAtPath/(allocation.get_height() / 30) + 1);       
     break;
     }   
  Gtk::EventBox::on_size_allocate(allocation);
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

  set_visible_window(false);


  Gtk::ScrolledWindow * MainScroller = Gtk::manage(new Gtk::ScrolledWindow);
  MainScroller->show();
  MainScroller->set_policy(Gtk::POLICY_ALWAYS, Gtk::POLICY_NEVER);
  add(*MainScroller);
  iconlist = new proto_icon;
  MainScroller->add(m_TreeView);
  m_TreeView.show();
  m_refTreeModel = Gtk::ListStore::create(*iconlist);
  m_TreeView.set_model(m_refTreeModel);



  m_TreeView.set_text_column(iconlist->m_col_all);
  m_TreeView.set_pixbuf_column(iconlist->m_col_icon);
  m_TreeView.set_orientation(Gtk::ORIENTATION_HORIZONTAL );
           m_TreeView.show_all_children();

  try { // make all those files into proto_icons
    Gnome::Vfs::DirectoryHandle::visit(
      parent->get_history(position),
      Gnome::Vfs::FILE_INFO_GET_MIME_TYPE |
      Gnome::Vfs::FILE_INFO_FORCE_FAST_MIME_TYPE |
      Gnome::Vfs::FILE_INFO_FOLLOW_LINKS ,
      Gnome::Vfs::DIRECTORY_VISIT_LOOPCHECK,
      sigc::mem_fun(*this,&DaIconModes::addEntry));
    }
  catch(const Gnome::Vfs::exception& ex){}

  m_TreeView.set_item_width( m_TreeView.get_icon_width()+ 235);
  m_TreeView.signal_button_press_event().connect_notify(
    sigc::bind<Glib::ustring>(
sigc::mem_fun(*this,&DaIconModes::icon_selected), "ha!"


// IconList->set_default_sort_func( sigc::mem_fun(*this, &DaIconModes::on_model_sort) );
));


  }

/**********************/

DaIconModes::~DaIconModes(){

  }

/**********************/

 void DaIconModes::SwitchHidden(guint pos){

position = pos;/*
if (parent->optShowHidden->get_active())
    parent->set_message("Showing Hidden Files");
else
    parent->set_message("Hidden Files are hidden again");
*/
  }

/**********************/

void DaIconModes::icon_selected(GdkEventButton* event, Glib::ustring file){
  if( (event->type == GDK_2BUTTON_PRESS) && (event->button == 1)){

    //RunFile(file);
    //m_Menu_Popup.popup(event->button, event->time);

    std::vector<Gtk::TreePath> refSelection =   m_TreeView.get_selected_items();
    if(refSelection.size() > 0){
      std::cout << "Running File :" << refSelection[0].to_string() << "\n";
      Gtk::TreeModel::Row row = *(m_refTreeModel->get_iter(refSelection[0]));
      std::cout <<  parent->get_history(position) << row[iconlist->m_col_name] << "\n";
      }
    }
  }


/**********************/
