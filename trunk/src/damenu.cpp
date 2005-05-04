
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
         // subdir->set_events(Gdk::BUTTON_RELEASE_MASK);

          subdir->signal_button_press_event().connect(
            sigc::bind<Glib::ustring,guint,bool>(
              sigc::mem_fun(*this, &DarimasenMenu::DaMenuSelect),
                 (path + refFileInfo->get_name()), position,false));

          SubSubCount = CountSubdir(path + refFileInfo->get_name());
          }
        else{
          subdir = Gtk::manage( new Gtk::MenuItem(ext + slash + refFileInfo->get_name() + slash + " "));


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

    void DarimasenMenu::copy(Glib::ustring path){}
    void DarimasenMenu::move(Glib::ustring path){}
    void DarimasenMenu::link(Glib::ustring path){}
    void DarimasenMenu::unlinkify(Glib::ustring path){}

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


