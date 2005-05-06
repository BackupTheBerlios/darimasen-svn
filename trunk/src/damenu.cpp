
/* Darimasen - damenu.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

/**********************/

#include "damenu.h"


/**********************/


// what actually builds the menus for each directory.
void DarimasenMenu::MenuForPath(
       int position,
       Glib::ustring path,
       Glib::ustring ext){

  int entry = 0;

MenuItemArray[position]->signal_activate().connect_notify(sigc::bind<guint,Glib::ustring>(sigc::mem_fun(*this,&DarimasenMenu:: selection_reset),position,path));

  if( position < depth || ext != ""){

    Gtk::MenuItem * subdir = Gtk::manage( new Gtk::MenuItem(menulevel[position] + " "));
    MenuArray[position]->attach(*subdir, 0 ,4, entry++, entry+1);
    subdir->show();

          subdir->set_events(Gdk::BUTTON_RELEASE_MASK);

    if ( ext != "" ){
          subdir->signal_button_press_event().connect(
            sigc::bind<Glib::ustring,guint,bool>(
              sigc::mem_fun(*this, &DarimasenMenu::DaMenuSelect),
                 (path + menulevel[position]).substr(0, (path + menulevel[position]).length() - 1),position,true));
      }
    else {
          subdir->signal_button_press_event().connect(
            sigc::bind<Glib::ustring,guint,bool>(
              sigc::mem_fun(*this, &DarimasenMenu::DaMenuSelect),
                 (path + menulevel[position]).substr(0, (path + menulevel[position]).length() - 1),position,false));
      }

    Gtk::SeparatorMenuItem * sep = Gtk::manage( new Gtk::SeparatorMenuItem());
    MenuArray[position]->attach(*sep, 0 ,4, entry++, entry+1);
    sep->show();
    }

  try{
    Gnome::Vfs::DirectoryHandle handle;

    if(ext == ""){
      handle.open(path, Gnome::Vfs::FILE_INFO_DEFAULT | Gnome::Vfs::FILE_INFO_FOLLOW_LINKS);
      }
    else{
      handle.open(path + slash + ext, Gnome::Vfs::FILE_INFO_DEFAULT | Gnome::Vfs::FILE_INFO_FOLLOW_LINKS);
      }

    bool file_exists = true;
    while(file_exists) {
      Glib::RefPtr<Gnome::Vfs::FileInfo> refFileInfo = handle.read_next(file_exists);
      if (refFileInfo->get_type() == Gnome::Vfs::FILE_TYPE_DIRECTORY
          && ( (refFileInfo->get_name().substr(0,1) != ".") || parent->optShowHidden->get_active() )
          && refFileInfo->get_name() != "."
          && refFileInfo->get_name() != ".."
          ){ 
        Gtk::MenuItem * subdir;
        Glib::ustring SubSubCount;
        if(ext == ""){
          subdir = Gtk::manage( new Gtk::MenuItem(refFileInfo->get_name() + slash + " "));
          subdir->set_events(Gdk::BUTTON_RELEASE_MASK);

          subdir->signal_button_press_event().connect(
            sigc::bind<Glib::ustring,guint,bool>(
              sigc::mem_fun(*this, &DarimasenMenu::DaMenuSelect),
                 (path + refFileInfo->get_name()), position,false));

          SubSubCount = CountSubdir(path + refFileInfo->get_name());
          }
        else{
          subdir = Gtk::manage( new Gtk::MenuItem(ext + slash + refFileInfo->get_name() + slash + " "));
          subdir->set_events(Gdk::BUTTON_RELEASE_MASK);

          subdir->signal_button_press_event().connect(
            sigc::bind<Glib::ustring,guint,bool>(
              sigc::mem_fun(*this, &DarimasenMenu::DaMenuSelect),
                (path + ext.substr(1) + slash + refFileInfo->get_name()), position,false));

          SubSubCount = CountSubdir(path + ext + slash + refFileInfo->get_name());
          }

        if (SubSubCount != "0"){
          Gtk::Label * SubSubLabel = Gtk::manage(new class Gtk::Label(SubSubCount + " \342\226\272",1,0.5));
          SubSubLabel->show();
          Gtk::MenuItem * subsubdir = Gtk::manage( new Gtk::MenuItem(*SubSubLabel));

          subsubdir->set_right_justified();

          subsubdir->set_events(Gdk::BUTTON_RELEASE_MASK);
          subsubdir->signal_button_release_event().connect(
            sigc::bind<int, Glib::ustring, Glib::ustring>(
              sigc::mem_fun(*this, &DarimasenMenu::SpecialMenuForPath),
                position, path, ext+ slash + refFileInfo->get_name()));

          MenuArray[position]->attach(*subsubdir, 3 ,4, entry, entry+1);
          subsubdir->show();
          MenuArray[position]->attach(*subdir, 0 ,3, entry++, entry+1);
          subdir->show();
          }
        else {
          MenuArray[position]->attach(*subdir, 0 ,4, entry++, entry+1);
          subdir->show();            
          }
        }
      }
    }
  catch(const Gnome::Vfs::exception& ex){}


  }

/**********************/

// the function called for sub-sub-directories - when an arrow is selected in another menu. 
bool DarimasenMenu::SpecialMenuForPath(GdkEventButton* event,
       int position,
       Glib::ustring path,
       Glib::ustring ext){

  MenuItemArray[position]->remove_submenu();
  delete MenuArray[position];
  MenuArray[position] =  new Gtk::Menu();
  MenuItemArray[position]->set_submenu(*MenuArray[position]);
  MenuForPath(position, path, ext);
  MenuItemArray[position]->select();

  needsRebuild[position] = true;

  return true;

  }

/**********************/

void DarimasenMenu:: selection_reset(guint position, Glib::ustring path){
  if ( needsRebuild[position] ){
    MenuItemArray[position]->remove_submenu();
    delete MenuArray[position];
    MenuArray[position] =  new Gtk::Menu();
    MenuItemArray[position]->set_submenu(*MenuArray[position]);
    MenuForPath(position, path, "");
    MenuItemArray[position]->select();

    needsRebuild[position] = false;
    }
}

/**********************/

//Decides the action of the menuItem, more sophisticated than the activate signal. 
//its comment should always be somewhere.

bool DarimasenMenu::DaMenuSelect(
      GdkEventButton* event,
      const Glib::ustring path,
      guint pos,
      bool first){

  if ((event->type == GDK_BUTTON_PRESS) && (event->button == 2) ) //middle
  {
  parent->newTab(path);
  return true;
  }
  if ((event->type == GDK_BUTTON_PRESS) && (event->button == 1) ) //left
  {
  parent->ChangeCurrentPath(path,true,first);
  return true;
  }
  if ((event->type == GDK_BUTTON_PRESS) && (event->button == 3) ) //right
  {

    for( int i=10; i > 0; i--)
      prompt.items().pop_back();


    prompt.items().push_back(
      Gtk::Menu_Helpers::MenuElem("Opening " + path + " from here consistantly leads to errors"));

    prompt.items().push_back(
      Gtk::Menu_Helpers::MenuElem("Open In new Tab",
        sigc::bind<Glib::ustring>(
          sigc::mem_fun(*parent,&Darimasen::newTab),path)));

    prompt.items().push_back(
      Gtk::Menu_Helpers::SeparatorElem());

    prompt.items().push_back(
      Gtk::Menu_Helpers::MenuElem("Copy...",
        sigc::bind<Glib::ustring>(
          sigc::mem_fun(*this,&DarimasenMenu::copy),path)));

    prompt.items().push_back(
      Gtk::Menu_Helpers::MenuElem("Move...",
        sigc::bind<Glib::ustring>(
          sigc::mem_fun(*this,&DarimasenMenu::move),path)));

    prompt.items().push_back(
      Gtk::Menu_Helpers::MenuElem("Link...",
        sigc::bind<Glib::ustring>(
          sigc::mem_fun(*this,&DarimasenMenu::link),path)));

    prompt.items().push_back(
      Gtk::Menu_Helpers::MenuElem("Delete...",
        sigc::bind<Glib::ustring>(
          sigc::mem_fun(*this,&DarimasenMenu::unlinkify),path)));

    prompt.items().push_back(
      Gtk::Menu_Helpers::SeparatorElem());

    prompt.items().push_back(
      Gtk::Menu_Helpers::MenuElem("Add to location menu",
        sigc::bind<Glib::ustring>(
          sigc::mem_fun(*this,&DarimasenMenu::bookmark),path)));

  //  prompt.signal_selection_done().connect_notify(sigc::mem_fun(prompt,&Gtk::Menu::hide));
  //  prompt.signal_selection_done().connect_notify(sigc::mem_fun(*MenuArray[pos],&Gtk::Menu::hide));
    prompt.signal_selection_done().connect_notify(sigc::mem_fun(*MenuItemArray[pos],&Gtk::MenuItem::deselect));
	
    prompt.popup(event->button, event->time);
  return true;
   }

  return false;

  //DaFileLister(); //5:45pm, 24 Dec 2004, it worked! // leave this comment    
  }


/**********************/

void DarimasenMenu::copy(Glib::ustring path){

  Gtk::Dialog copyDialogue;
  Gtk::Button * cancelbutton2 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
  Gtk::Button * okbutton2 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   
  Gtk::RadioButton::Group _RadioBGroup_radiobutton4;
  Gtk::RadioButton * radiobutton4 = Gtk::manage(
    new class Gtk::RadioButton(_RadioBGroup_radiobutton4, "Copy Directory to ... (cp .)"));
  Gtk::RadioButton * radiobutton5 = Gtk::manage(
    new class Gtk::RadioButton(_RadioBGroup_radiobutton4, "Copy files in directory to ... (cp *)"));
  Gtk::RadioButton * radiobutton6 = Gtk::manage(
    new class Gtk::RadioButton(_RadioBGroup_radiobutton4, "Copy everything in directory to ... (cp -R *)"));
  Gtk::Entry * entry2 = Gtk::manage(new class Gtk::Entry());
  cancelbutton2->set_flags(Gtk::CAN_FOCUS);
  cancelbutton2->set_flags(Gtk::CAN_DEFAULT);
  okbutton2->set_flags(Gtk::CAN_FOCUS);

  copyDialogue.get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
  radiobutton4->set_flags(Gtk::CAN_FOCUS);
  radiobutton5->set_flags(Gtk::CAN_FOCUS);
  radiobutton6->set_flags(Gtk::CAN_FOCUS);
  entry2->set_flags(Gtk::CAN_FOCUS);
  entry2->set_editable(true);
  entry2->set_text(path);
  copyDialogue.get_vbox()->pack_start(*radiobutton4, Gtk::PACK_SHRINK, 0);
  copyDialogue.get_vbox()->pack_start(*radiobutton5, Gtk::PACK_SHRINK, 0);
  copyDialogue.get_vbox()->pack_start(*radiobutton6, Gtk::PACK_SHRINK, 0);
  copyDialogue.get_vbox()->pack_start(*entry2, Gtk::PACK_SHRINK, 0);
  copyDialogue.set_title("Copy (Directory)");
  copyDialogue.set_modal(true);

  copyDialogue.set_resizable(true);
  copyDialogue.add_action_widget(*cancelbutton2, -6);
  copyDialogue.add_action_widget(*okbutton2, -5);
  copyDialogue.show_all_children();

  switch(copyDialogue.run()){
    case(Gtk::RESPONSE_OK):{
      std::cout << path << "moved\n";
      return;
      }
    default:{
      std::cout << path << "not moved\n";
      return;
      }
    }
  }

/**********************/

void DarimasenMenu::move(Glib::ustring path){

  Gtk::Dialog moveDialogue;
  Gtk::Button * cancelbutton1 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
  Gtk::Button * okbutton1 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   
  Gtk::RadioButton::Group _RadioBGroup_radiobutton1;
  Gtk::RadioButton * radiobutton1 = Gtk::manage(new class Gtk::RadioButton(
    _RadioBGroup_radiobutton1, "Move " + path + " to ... (mv .)"));
  Gtk::RadioButton * radiobutton2 = Gtk::manage(new class Gtk::RadioButton(
    _RadioBGroup_radiobutton1, "Move files in directory to... (mv *)"));
  Gtk::RadioButton * radiobutton3 = Gtk::manage(new class Gtk::RadioButton(
    _RadioBGroup_radiobutton1, "Move everything in directory to... (mv -R *)"));
  Gtk::Entry * entry1 = Gtk::manage(new class Gtk::Entry());
  cancelbutton1->set_flags(Gtk::CAN_FOCUS);
  cancelbutton1->set_flags(Gtk::CAN_DEFAULT);
  okbutton1->set_flags(Gtk::CAN_FOCUS);
  moveDialogue.get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
  radiobutton1->set_flags(Gtk::CAN_FOCUS);
  radiobutton2->set_flags(Gtk::CAN_FOCUS);
  radiobutton3->set_flags(Gtk::CAN_FOCUS);
  entry1->set_flags(Gtk::CAN_FOCUS);
  entry1->set_editable(true);
  entry1->set_text(path);
  moveDialogue.get_vbox()->pack_start(*radiobutton1, Gtk::PACK_SHRINK, 0);
  moveDialogue.get_vbox()->pack_start(*radiobutton2, Gtk::PACK_SHRINK, 0);
  moveDialogue.get_vbox()->pack_start(*radiobutton3, Gtk::PACK_SHRINK, 0);
  moveDialogue.get_vbox()->pack_start(*entry1, Gtk::PACK_SHRINK, 0);
  moveDialogue.set_title("Move (Directory)");
  moveDialogue.set_modal(true);
  moveDialogue.set_resizable(true);
  moveDialogue.property_destroy_with_parent().set_value(true);
  moveDialogue.add_action_widget(*cancelbutton1, -6);
  moveDialogue.add_action_widget(*okbutton1, -5);
  moveDialogue.show_all_children();

  switch(moveDialogue.run()){
    case(Gtk::RESPONSE_OK):{
      std::cout << path << "moved\n";
      return;
      }
    default:{
      std::cout << path << "not moved\n";
      return;
      }
    }
  }


/**********************/

void DarimasenMenu::link(Glib::ustring path){
  Gtk::Dialog linkDialogue;
  Gtk::Button * cancelbutton3 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
  Gtk::Button * okbutton3 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
  Gtk::Label * label1 = Gtk::manage(new class Gtk::Label("Create link to "+ path +" at ..."));
  Gtk::Entry * entry3 = Gtk::manage(new class Gtk::Entry());
  cancelbutton3->set_flags(Gtk::CAN_FOCUS);
  cancelbutton3->set_flags(Gtk::CAN_DEFAULT);
  okbutton3->set_flags(Gtk::CAN_FOCUS);
  linkDialogue.get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
  label1->set_alignment(0,0.5);
  label1->set_padding(2,2);
  label1->set_justify(Gtk::JUSTIFY_LEFT);
  label1->set_line_wrap(true);
  label1->set_use_markup(false);
  label1->set_selectable(false);
  entry3->set_flags(Gtk::CAN_FOCUS);
  entry3->set_visibility(true);
  entry3->set_editable(true);
  entry3->set_text(path);
  entry3->set_activates_default(false);
  linkDialogue.get_vbox()->pack_start(*label1, Gtk::PACK_SHRINK, 0);
  linkDialogue.get_vbox()->pack_start(*entry3, Gtk::PACK_SHRINK, 0);
  linkDialogue.set_title("Link (Directory)");
  linkDialogue.set_modal(true);
  linkDialogue.set_resizable(true);
  linkDialogue.property_destroy_with_parent().set_value(true);
  linkDialogue.add_action_widget(*cancelbutton3, -6);
  linkDialogue.add_action_widget(*okbutton3, -5);

  linkDialogue.show_all_children();

  switch(linkDialogue.run()){
    case(Gtk::RESPONSE_OK):{
      std::cout << "link created " + path << "\n";
      return;
      }
    default:{
      std::cout << "No link created " + path << "\n";
      return;
      }
    }
  }

/**********************/


void DarimasenMenu::unlinkify(Glib::ustring path){
  Gtk::Dialog unlinkifyDialogue;
  Gtk::Button * cancelbutton4 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
  Gtk::Button * okbutton4 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
  Gtk::Label * label2 = Gtk::manage(
    new class Gtk::Label("Are you sure you want to delete " + path + " and its contents?"));
  cancelbutton4->set_flags(Gtk::CAN_FOCUS);
  cancelbutton4->set_flags(Gtk::CAN_DEFAULT);
  okbutton4->set_flags(Gtk::CAN_FOCUS);
  unlinkifyDialogue.get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
  label2->set_alignment(0,0.5);
  label2->set_padding(2,2);
  label2->set_justify(Gtk::JUSTIFY_LEFT);
  label2->set_line_wrap(true);
  unlinkifyDialogue.get_vbox()->pack_start(*label2, Gtk::PACK_SHRINK, 0);
  unlinkifyDialogue.set_title("Delete (Directory)");
  unlinkifyDialogue.set_modal(true);
  unlinkifyDialogue.property_window_position().set_value(Gtk::WIN_POS_NONE);
  unlinkifyDialogue.set_resizable(true);
  unlinkifyDialogue.property_destroy_with_parent().set_value(true);
  unlinkifyDialogue.add_action_widget(*cancelbutton4, -6);
  unlinkifyDialogue.add_action_widget(*okbutton4, -5);
  unlinkifyDialogue.show_all_children();

  switch(unlinkifyDialogue.run()){
    case(Gtk::RESPONSE_OK):{
      std::cout << "Deleted " + path << "\n";
      return;
      }
    default:{
      std::cout << "No deletion " + path << "\n";
      return;
      }
    }
  }

    void DarimasenMenu::bookmark(Glib::ustring path){}

/**********************/

// provides the numbers by the arrows. 
Glib::ustring DarimasenMenu::CountSubdir(const Glib::ustring& path){
  int j=0;
  try{
    Gnome::Vfs::DirectoryHandle handle;
    handle.open(path, Gnome::Vfs::FILE_INFO_DEFAULT | Gnome::Vfs::FILE_INFO_FOLLOW_LINKS);

    bool file_exists = true;
    while(file_exists){

      Glib::RefPtr<Gnome::Vfs::FileInfo> refFileInfo = handle.read_next(file_exists);
      if (refFileInfo->get_type() == Gnome::Vfs::FILE_TYPE_DIRECTORY
        && ( (refFileInfo->get_name().substr(0,1) != ".") || parent->optShowHidden->get_active())
        && refFileInfo->get_name() != "."
        && refFileInfo->get_name() != ".."){
        j++;
        }
      }
    }
  catch(const Gnome::Vfs::exception& ex){}
  return int2ustr(j);

}

/**********************/

// parses the path, builds the menu bar. 
DarimasenMenu::DarimasenMenu(const Glib::ustring & path, Darimasen& Myparent, guint pos){

  depth = 0;

  Glib::ustring shortpath = path; //home = getenv("HOME");

  parent = &Myparent;
  
  int startPos = 0 , i = 0;


  if ( path.find(getenv("HOME")) != Glib::ustring::npos ){
    shortpath.replace(0,((Glib::ustring)getenv("HOME")).length(),"~");
    }

  while ( (shortpath.find(slash,startPos) != Glib::ustring::npos) ) {
    startPos = shortpath.find(slash,startPos) + 1; depth++;
    }

  menulevel =  new Glib::ustring[depth+1];
  needsRebuild = new bool[depth+1];
  for( int i=0; i<=depth; i++)
        needsRebuild[i] = false;


  startPos = 0;
  for(i = 0; shortpath.find(slash,startPos) !=  Glib::ustring::npos ; i++ ){
    menulevel[i] = shortpath.substr(startPos,shortpath.find(slash,startPos) - startPos + 1 );
    startPos = shortpath.find(slash,startPos) + 1;
    };
  menulevel[depth] = ".";

  MenuArray = new Gtk::Menu*[depth+1];
  MenuItemArray = new Gtk::MenuItem*[depth+1];

  for(int c = 0; c < depth+1; c++){
    MenuArray[c] =  new Gtk::Menu();
    }
 
    Glib::ustring subin = CountSubdir(path);
    if ( subin != "0" ){
      MenuItemArray[depth] =  new Gtk::MenuItem( subin + " \342\226\272" );
      MenuItemArray[depth]->show();
      MenuItemArray[depth]->set_submenu(*MenuArray[depth]);
      prepend(*MenuItemArray[depth]);
      MenuForPath(depth, path, "");
      }
  
  Glib::ustring crop = path;


  for(i = depth - 1; i>0; i--){
    crop = crop.substr(0, crop.rfind(slash,crop.length()-2)) + slash;

    MenuItemArray[i] = new Gtk::MenuItem(underscoreSafe(menulevel[i]) );
    MenuItemArray[i]->show();
    MenuItemArray[i]->set_submenu(*MenuArray[i]);
    prepend(*MenuItemArray[i]);
    MenuForPath(i, crop, "");
    }

  MenuItemArray[i] = new Gtk::MenuItem(underscoreSafe(menulevel[i] ));
  MenuItemArray[i]->show();
  prepend(*MenuItemArray[i]);



// root stuff
    MenuItemArray[0]->set_submenu(*MenuArray[0]);
    MenuItemArray[0]->show();
    MenuArray[0]->show();

    Gtk::MenuItem * subdir = Gtk::manage( new Gtk::MenuItem("/ (root)"));
    MenuArray[0]->attach(*subdir, 0 ,1, 0, 1);
    subdir->show();

          subdir->set_events(Gdk::BUTTON_RELEASE_MASK);

          subdir->signal_button_press_event().connect(
            sigc::bind<Glib::ustring,guint,bool>(
              sigc::mem_fun(*this, &DarimasenMenu::DaMenuSelect),
                 "/",pos,false));



    Gtk::MenuItem * subdir2 = Gtk::manage( new Gtk::MenuItem("~ (home)"));
    MenuArray[0]->attach(*subdir2, 0 ,1, 1, 2);
    subdir2->show();

          subdir2->set_events(Gdk::BUTTON_RELEASE_MASK);

          subdir2->signal_button_press_event().connect(
            sigc::bind<Glib::ustring,guint,bool>(
              sigc::mem_fun(*this, &DarimasenMenu::DaMenuSelect),
                getenv("HOME"),pos,false));

    Gtk::SeparatorMenuItem * sep = Gtk::manage( new Gtk::SeparatorMenuItem());
    MenuArray[0]->attach(*sep, 0 ,1, 2, 3);
    sep->show();

    Gtk::MenuItem * subdir3 = Gtk::manage( new Gtk::MenuItem("Add a location to this list"));
    MenuArray[0]->attach(*subdir3, 0 ,1, 3, 4);
    subdir3->set_sensitive(false);
    subdir3->show();


  show();

  }

/**********************/

// cleanup on asile 9. More might me Gtk::manageable...
DarimasenMenu::~DarimasenMenu(){
  for(int c = 0; c < depth+1; c++){
    delete MenuArray[c];
    }
  delete menulevel;
  delete needsRebuild;
  for (int i = 0; i <=depth; i++){
    delete MenuArray[i];
    delete MenuItemArray[i];
    }
  
  }

/**********************/


