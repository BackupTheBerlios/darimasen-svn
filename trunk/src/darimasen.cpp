/* Darimasen - darimasen.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

/**********************/

#include "darimasen.h"
#include <iostream>

/**********************/


// what actually builds the menus for each directory.
void Darimasen::DarimasenMenu::MenuForPath(
       int position,
       Glib::ustring path,
       Glib::ustring ext){

  int entry = 0;

  


  if( position < depth || ext != ""){

    Gtk::MenuItem * subdir = Gtk::manage( new Gtk::MenuItem(menulevel[position] + " "));
    MenuArray[position]->attach(*subdir, 0 ,4, entry++, entry+1);
    subdir->show();

          subdir->set_events(Gdk::BUTTON_RELEASE_MASK);

          subdir->signal_button_press_event().connect(
            sigc::bind<Glib::ustring>(
              sigc::mem_fun(*this, &Darimasen::DarimasenMenu::DaMenuSelect),
                 (path + menulevel[position]).substr(0, (path + menulevel[position]).length() - 1)));

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
            sigc::bind<Glib::ustring>(
              sigc::mem_fun(*this, &Darimasen::DarimasenMenu::DaMenuSelect),
                 (path + refFileInfo->get_name())));

          SubSubCount = CountSubdir(path + refFileInfo->get_name());
          }
        else{
          subdir = Gtk::manage( new Gtk::MenuItem(ext + slash + refFileInfo->get_name() + slash + " "));
          subdir->set_events(Gdk::BUTTON_RELEASE_MASK);

          subdir->signal_button_press_event().connect(
            sigc::bind<Glib::ustring>(
              sigc::mem_fun(*this, &Darimasen::DarimasenMenu::DaMenuSelect),
                (path + ext.substr(1) + slash + refFileInfo->get_name())));

          SubSubCount = CountSubdir(path + ext + slash + refFileInfo->get_name());
          }

        if (SubSubCount != "0"){
          Gtk::Label * SubSubLabel = Gtk::manage(new class Gtk::Label(SubSubCount + " \342\226\272",1,0.5));
          SubSubLabel->show();
          Gtk::MenuItem * subsubdir = Gtk::manage( new Gtk::MenuItem(*SubSubLabel));
          subsubdir->set_right_justified();

          subsubdir->signal_activate().connect(
            sigc::bind<int, Glib::ustring, Glib::ustring>(
              sigc::mem_fun(*this, &Darimasen::DarimasenMenu::SpecialMenuForPath),
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
void Darimasen::DarimasenMenu::SpecialMenuForPath(
       int position,
       Glib::ustring path,
       Glib::ustring ext){

MenuArray[position]->imExtended = true;
    extended = position; 
  std::cout << extended << "/n";

  MenuItemArray[position]->remove_submenu();
  delete MenuArray[position];
  MenuArray[position] = Gtk::manage( new DirectoryMenu(*this));



  MenuItemArray[position]->set_submenu(*MenuArray[position]);
  MenuForPath(position, path, ext);



 // MenuArray[position]->signal_deactivate().connect(
 //   sigc::bind<guint, Glib::ustring>(sigc::mem_fun(*this, &Darimasen::DarimasenMenu::offClick),position, path) );


 // MenuArray[position]->popup( *this,  *MenuItemArray[position], sigc::mem_fun(*this,& Darimasen::DarimasenMenu::on_popup_menu_position), 1, gtk_get_current_event_time());
  MenuArray[position]->popup( 1, gtk_get_current_event_time());

  }

/**********************

void Darimasen::DarimasenMenu::on_popup_menu_position(int& x, int& y, bool& push_in){
x = 200;
y = 200;
push_in = true;
}


/**********************/

void Darimasen::DarimasenMenu::offClick(){

  MenuItemArray[extended]->remove_submenu();
  delete MenuArray[extended];
  MenuArray[extended] = Gtk::manage( new DirectoryMenu(*this));
  MenuItemArray[extended]->set_submenu(*MenuArray[extended]);
  MenuForPath(extended, parent->history[extended].top() , "");
extended = -1;
}

/**********************/

//Decides the action of the menuItem, more sophisticated than the activate signal. 
//its comment should always be somewhere.

bool Darimasen::DarimasenMenu::DaMenuSelect(GdkEventButton* event, const Glib::ustring path){

  if ((event->type == GDK_BUTTON_PRESS) && (event->button == 2) ) //middle
  {
  parent->newTab(path);
  return true;
  }
  if ((event->type == GDK_BUTTON_PRESS) && (event->button == 1) ) //left
  {
  parent->ChangeCurrentPath(path);
  return true;
  }
  if ((event->type == GDK_BUTTON_PRESS) && (event->button == 3) ) //right
  {
    Gtk::Menu * m_Menu_Popup = Gtk::manage( new Gtk::Menu);
    Gtk::Menu::MenuList& menulist = m_Menu_Popup->items();

    menulist.push_back( Gtk::Menu_Helpers::MenuElem("Open In new Tab"));
    m_Menu_Popup->popup(event->button, event->time);
  return true;
   }

  return false;

  //DaFileLister(); //5:45pm, 24 Dec 2004, it worked! // leave this comment    
  }

/**********************/

// provides the numbers by the arrows. 
Glib::ustring Darimasen::DarimasenMenu::CountSubdir(const Glib::ustring& path){
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

void  Darimasen::DarimasenMenu::signal_deactivate (){
std::cout << "on_unmap_event(GdkEventAny* event)\n";

}

/**********************/

// parses the path, builds the menu bar. 
Darimasen::DarimasenMenu::DarimasenMenu(const Glib::ustring path, Darimasen& Myparent){
  depth = 0;
  extended = -1;
//usingSpecial = false;
  //showHidden = false;
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

  startPos = 0;
  for(i = 0; shortpath.find(slash,startPos) !=  Glib::ustring::npos ; i++ ){
    menulevel[i] = shortpath.substr(startPos,shortpath.find(slash,startPos) - startPos + 1 );
    startPos = shortpath.find(slash,startPos) + 1;
    };
  menulevel[depth] = ".";

  MenuArray = new DirectoryMenu*[depth+1];
  MenuItemArray = new Gtk::MenuItem*[depth+1];

  for(int c = 0; c < depth+1; c++){
    MenuArray[c] =  new DirectoryMenu(*this);
    }
 
    Glib::ustring subin = CountSubdir(path);
    if ( subin != "0" ){
      MenuItemArray[depth] =  Gtk::manage(new Gtk::MenuItem( subin + " \342\226\272" ));
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
            sigc::bind<Glib::ustring>(
              sigc::mem_fun(*this, &Darimasen::DarimasenMenu::DaMenuSelect),
                 "/"));



    Gtk::MenuItem * subdir2 = Gtk::manage( new Gtk::MenuItem("~ (home)"));
    MenuArray[0]->attach(*subdir2, 0 ,1, 1, 2);
    subdir2->show();

          subdir2->set_events(Gdk::BUTTON_RELEASE_MASK);

          subdir2->signal_button_press_event().connect(
            sigc::bind<Glib::ustring>(
              sigc::mem_fun(*this, &Darimasen::DarimasenMenu::DaMenuSelect),
                getenv("HOME")));

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
Darimasen::DarimasenMenu::~DarimasenMenu(){
  for(int c = 0; c < depth+1; c++){
    delete MenuArray[c];
    }
  delete menulevel;
  delete MenuArray;
  delete MenuItemArray;
  }

/**********************/

// anything prefixed with 'f' corresponds with a button. 'nuff said.
void Darimasen::fNewTab(){
    std::stack<Glib::ustring> empty;
    history.push_back(empty);

  history[history.size() -1].push(getenv("HOME") + slash);
  addTab(Tabber->get_n_pages());

  }

/**********************/

// ...and there was already one with an f.
void Darimasen::newTab(Glib::ustring newpath){
    std::stack<Glib::ustring> empty;
    history.push_back(empty);

if( newpath.substr(newpath.length() - 1) != slash)
   newpath += slash;

  history[history.size() -1].push(newpath);
  addTab(Tabber->get_n_pages());

  }

/**********************/

// this is for a signal handler.
void Darimasen::tabberSwitched(GtkNotebookPage* sig, guint n){
  DarimasenMenuContainer->remove();

  DaMenu = Gtk::manage(new DarimasenMenu(history[n].top(), *this));
  DarimasenMenuContainer->add(*DaMenu);

  if (history[n].size() == 1)
    BackButton->set_sensitive(false);
  else
    BackButton->set_sensitive(true);

  }

/**********************/

// all the stuff for a well rounded tab
void Darimasen::addTab(guint pos){

  if( Tabber->get_n_pages() == 0 )
    Tabber->set_show_tabs(false);
  else
    Tabber->set_show_tabs(true);

  Gtk::Image * xed = Gtk::manage(
    new Gtk::Image("/usr/share/icons/hicolor/16x16/stock/generic/stock_close.png"));
  xed->show();

  Gtk::Label * tabNum = Gtk::manage(new Gtk::Label(

  history[pos].top()
//path.substr(path.rfind(slash, path.length() -2)+1) + "  "
));
  Gtk::HBox * arrangement= Gtk::manage(new Gtk::HBox()) ;
  Gtk::Button * closeButton = Gtk::manage( new Gtk::Button());

  closeButton->add(*xed);
  closeButton->set_relief(Gtk::RELIEF_NONE); 
  closeButton->show();

  closeButton->signal_clicked().connect(
    sigc::bind<guint>(sigc::mem_fun(*this, &Darimasen::removeTab),pos) );


  tabNum->show();
  arrangement->show();

  arrangement->pack_start(*tabNum);
  arrangement->pack_end(*closeButton);

  Gtk::EventBox * MainEventBox = Gtk::manage(new class Gtk::EventBox);

  Gdk::Color m_Color;
  m_Color.set_rgb(65535, 65535, 65535);
  MainEventBox->modify_bg(Gtk::STATE_NORMAL, m_Color);
  MainEventBox->show();

  DaIconModes * foo;
  foo = new DaIconModes(  history[pos].top(), *this);



  if ( IconModeList.size() == pos ){
    IconModeList.push_back(foo);
    }
  else {
    delete IconModeList[pos];
    IconModeList[pos] = foo;
    }

  Gtk::ScrolledWindow * MainScroller = Gtk::manage(new Gtk::ScrolledWindow);
  MainScroller->show();
  MainScroller->set_shadow_type(Gtk::SHADOW_NONE);
  MainScroller->add(*MainEventBox);
  MainEventBox->add(*foo);
  MainScroller->set_policy(Gtk::POLICY_ALWAYS, Gtk::POLICY_AUTOMATIC);
  foo->show();
  Tabber->insert_page( *MainScroller, *arrangement, pos);

  }
/**********************/

// changing directories actually is making a new tab to substitute for the old one.
void Darimasen::ChangeCurrentPath(Glib::ustring pathin){
  guint nth = Tabber->get_current_page();


  //there is a circumstance that only the menu should be rebuilt...
  if( pathin !=  history[nth].top()){
    if (pathin.substr(pathin.length()-1) != "/")
      history[nth].push(pathin + slash);
    else
      history[nth].push(pathin);
    BackButton->set_sensitive(true);

    Tabber->remove_page(nth);
    addTab(nth);
    Tabber->set_current_page(nth);
    }


  DarimasenMenuContainer->remove();
  DaMenu = Gtk::manage( new DarimasenMenu(history[nth].top(), *this));
  DarimasenMenuContainer->add(*DaMenu);

  }

/**********************/

// removing a tab has some cleanup involved...
void Darimasen::removeTab(guint pos){

  if( Tabber->get_n_pages() == 2 )
    Tabber->set_show_tabs(false);
  else
    Tabber->set_show_tabs(true);

  guint tmp = pos;
  Tabber->remove_page(pos);

  history.erase(history.begin()+pos,history.begin()+pos+1 );
  delete IconModeList[pos];
  IconModeList.erase(IconModeList.begin()+pos,IconModeList.begin()+pos+1 );
  }


/**********************/

Darimasen::Darimasen(std::vector<Glib::ustring> paths){
  set_title("Darimasen");
  set_default_size(500, 330);

  add(VerticalOrganizer);
  VerticalOrganizer.show();

  VerticalOrganizer.pack_start(TopBar, Gtk::PACK_SHRINK);
  TopBar.show();

  TopBar.set_toolbar_style(Gtk::TOOLBAR_ICONS); 


  Gtk::ToolItem * CompactMenuContainer = Gtk::manage(new Gtk::ToolItem);
  TopBar.append(*CompactMenuContainer);
  CompactMenuContainer->show();

  Gtk::MenuBar * CompactMenu = Gtk::manage(new Gtk::MenuBar);
  Gtk::Menu * m_Menu_File = Gtk::manage(new Gtk::Menu);
  Gtk::Menu::MenuList& menulist = m_Menu_File->items();

  CompactMenu->items().push_back( Gtk::Menu_Helpers::MenuElem("\342\226\274", *m_Menu_File) );

  // glade-- told me to do it....
  menulist.push_back(Gtk::Menu_Helpers::CheckMenuElem(
    "Show Hidden",Gtk::AccelKey(GDK_H, Gdk::CONTROL_MASK), sigc::mem_fun(*this, &Darimasen::fShowHidden)));
  optShowHidden = (Gtk::CheckMenuItem *)&menulist.back();

  menulist.push_back(Gtk::Menu_Helpers::MenuElem(
    "Print history to Terminal",Gtk::AccelKey(GDK_P, Gdk::CONTROL_MASK), sigc::mem_fun(*this, &Darimasen::fPrintHist)));

  menulist.push_back( Gtk::Menu_Helpers::MenuElem("About...",
    sigc::mem_fun(*this, &Darimasen::fAbout) ) );

  menulist.push_back( Gtk::Menu_Helpers::SeparatorElem() ) ;

  menulist.push_back( Gtk::Menu_Helpers::StockMenuElem(Gtk::Stock::QUIT,
    sigc::mem_fun(*this, &Darimasen::fQuit) ) );
    
  TopBar.set_show_arrow(false);
  CompactMenuContainer->add(*CompactMenu); 
  CompactMenu->show();


  Gtk::SeparatorToolItem * sep1  = Gtk::manage(new Gtk::SeparatorToolItem);
  TopBar.append(*sep1);
  sep1->show();

  DarimasenMenuContainer = Gtk::manage(new Gtk::ToolItem);
  TopBar.append(*DarimasenMenuContainer);
  DarimasenMenuContainer->set_expand(true);
  DarimasenMenuContainer->show();

  Gtk::SeparatorToolItem * sep2  = Gtk::manage(new Gtk::SeparatorToolItem);
  TopBar.append(*sep2);
  sep2->show();

  BackButton = Gtk::manage(new Gtk::ToolButton(Gtk::StockID("gtk-go-back")));
  BackButton->signal_clicked().connect(sigc::mem_fun(*this, &Darimasen::fBack));
  TopBar.append(*BackButton);
  BackButton->set_sensitive(false);
  BackButton->show();
  
  Gtk::ToolButton * ChangeIconMode = Gtk::manage(new Gtk::ToolButton(Gtk::StockID("gtk-convert")));
  //ChangeIconMode->signal_clicked().connect(sigc::mem_fun(*this, &Darimasen::fChangeIconMode));
  TopBar.append(*ChangeIconMode);
  ChangeIconMode->set_sensitive(false);
  ChangeIconMode->show();
   
  Gtk::ToggleToolButton * ViewTree = Gtk::manage(new Gtk::ToggleToolButton(Gtk::StockID("gtk-index")));
  //ViewTree->signal_clicked().connect(sigc::mem_fun(*this, &Darimasen::fViewTree));
  TopBar.append(*ViewTree);
  ViewTree->set_sensitive(false);
  ViewTree -> show();

  Gtk::ToolButton * NewTab = Gtk::manage(new Gtk::ToolButton(Gtk::StockID("gtk-new")));
  NewTab->signal_clicked().connect(sigc::mem_fun(*this, &Darimasen::fNewTab));
  TopBar.append(*NewTab);
  NewTab->show();

  VerticalOrganizer.add(HideTreePane);
  HideTreePane.show();

  HideTreePane.pack1(TreeScroller, Gtk::SHRINK);

  Tabber = Gtk::manage(new Gtk::Notebook);
  Tabber->signal_switch_page().connect(sigc::mem_fun(*this, &Darimasen::tabberSwitched), true);
  Tabber->set_show_border(false);
  Tabber->show();

  HideTreePane.pack2(*Tabber, Gtk::EXPAND|Gtk::SHRINK);
   
  Info.set_has_resize_grip();
  Info.show();
  VerticalOrganizer.pack_start(Info, Gtk::PACK_SHRINK);

  show();

  for(int i = 0; i < paths.size(); i++){
    std::stack<Glib::ustring> empty;
    history.push_back(empty);
    history[i].push(paths[i]);

    addTab(Tabber->get_n_pages());
    }


 }

/**********************/

Darimasen::~Darimasen(){
  fQuit();
  }

/**********************/

void Darimasen::fQuit(){
  DarimasenMenuContainer->remove();
  hide();
  }

/**********************/

void Darimasen::fAbout(){



  Glib::RefPtr<Gdk::Pixbuf> loader=  
    Gdk::Pixbuf::create_from_file( 
      ((Glib::ustring)DATADIR + (Glib::ustring)"/pixmaps/darimasenInfo.svg" ));



   Gtk::Dialog aboutDarimasen;

   Gtk::Button * closebutton1 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-close")));
   Gtk::Image * image1 = Gtk::manage(new class Gtk::Image(loader));


   aboutDarimasen.get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);


   aboutDarimasen.get_vbox()->set_homogeneous(false);
   aboutDarimasen.get_vbox()->set_spacing(10);
   aboutDarimasen.get_vbox()->pack_start(*image1);

   aboutDarimasen.set_title("About Darimasen");
   aboutDarimasen.set_modal(true);
   aboutDarimasen.set_resizable(false);
   aboutDarimasen.add_action_widget(*closebutton1, -7);

  aboutDarimasen.show_all_children();
  aboutDarimasen.run();
  }

/**********************/

void Darimasen::fShowHidden(){


  DarimasenMenuContainer->remove();
  DaMenu = Gtk::manage( new DarimasenMenu(history[Tabber->get_current_page()].top(), *this));
  DarimasenMenuContainer->add(*DaMenu);



for( int i = 0; IconModeList.size() > i; i++)
  IconModeList[i]->SwitchHidden();

  }

/**********************/

void Darimasen::fBack(){
  history[Tabber->get_current_page()].pop();
  Glib::ustring tmp = history[Tabber->get_current_page()].top();
  history[Tabber->get_current_page()].pop();
  ChangeCurrentPath(tmp);

  if (history[Tabber->get_current_page()].size() == 1)
    BackButton->set_sensitive(false);
 }

/**********************/

void Darimasen::fPrintHist(){
  std::vector< std::stack<Glib::ustring> > destroy = history;
  
  std::cout << "There are " << destroy.size() << " tabs here.\n";
  
  for (int i = 0; i < destroy.size(); i++){
    for (; destroy[i].size() > 0; destroy[i].pop()){
      std::cout << "history[" << i << "].top() = " << destroy[i].top() << "\n";
      }
    }
  }

/**********************/

  void Darimasen::set_message(Glib::ustring in){
   Info.pop();
   Info.push(in);
}
/**********************/
