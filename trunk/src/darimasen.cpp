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



  if( position < depth ){

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


  MenuItemArray[position]->remove_submenu();
  delete MenuArray[position];
  MenuArray[position] = Gtk::manage( new Gtk::Menu);


  MenuItemArray[position]->set_submenu(*MenuArray[position]);
  MenuForPath(position, path, ext);

  MenuItemArray[position]->select(); // just using this seems buggy for bringing up the new menu. 
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

// parses the path, builds the menu bar. 
Darimasen::DarimasenMenu::DarimasenMenu(const Glib::ustring path, Darimasen& Myparent){
  depth = 0;
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

  menulevel =  new Glib::ustring[depth];

  startPos = 0;
  for(i = 0; shortpath.find(slash,startPos) !=  Glib::ustring::npos ; i++ ){
    menulevel[i] = shortpath.substr(startPos,shortpath.find(slash,startPos) - startPos + 1 );
    startPos = shortpath.find(slash,startPos) + 1;
    };

  MenuArray = new Gtk::Menu*[depth+1];
  MenuItemArray = new Gtk::MenuItem*[depth+1];

  for(int c = 0; c < depth+1; c++){
    MenuArray[c] =  new Gtk::Menu;
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

#ifdef WIN32
  history[history.size() -1].push(getenv("USERPROFILE") + slash);
  addTab(getenv("USERPROFILE") + slash, Tabber->get_n_pages());
#else
  history[history.size() -1].push(getenv("HOME") + slash);
  addTab(getenv("HOME") + slash, Tabber->get_n_pages());
#endif
  }

/**********************/

// ...and there was already one with an f.
void Darimasen::newTab(Glib::ustring newpath){
    std::stack<Glib::ustring> empty;
    history.push_back(empty);


  history[history.size() -1].push(newpath + slash);
  addTab(newpath + slash, Tabber->get_n_pages());

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
void Darimasen::addTab(Glib::ustring path, guint pos){

  if( Tabber->get_n_pages() == 0 )
    Tabber->set_show_tabs(false);
  else
    Tabber->set_show_tabs(true);

  Gtk::Image * xed = Gtk::manage(
    new Gtk::Image("/usr/share/icons/hicolor/16x16/stock/generic/stock_close.png"));
  xed->show();

  Gtk::Label * tabNum = Gtk::manage(new Gtk::Label(path.substr(path.rfind(slash, path.length() -2)+1) + "  "));
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
  foo = new DaIconModes(path, *this);

hackishUnhide[pos] = foo;
  //IconModeList.push(*foo);
//if the current tab number is greater than those that actually exist,
//push_back. Otherwise, change the one at pos. 
 /* if (IconModeList.size() == pos){
    std::cout << "push modelist\n";
    IconModeList.push_back(*foo);
    }
  else{
    IconModeList.erase
      history.erase(history.begin()+pos,history.begin()+pos+1 );
    
    }*/

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


  if (pathin.substr(pathin.length()-1) != "/")
    history[nth].push(pathin + slash);
  else
    history[nth].push(pathin);

  BackButton->set_sensitive(true);

  DarimasenMenuContainer->remove();
  DaMenu = Gtk::manage( new DarimasenMenu(history[nth].top(), *this));
  DarimasenMenuContainer->add(*DaMenu);

  Tabber->remove_page(nth);

  addTab(history[nth].top(), nth);
  Tabber->set_current_page(nth);
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
delete hackishUnhide[pos];
  }


/**********************/

Darimasen::Darimasen(std::vector<Glib::ustring> paths){
  set_title("Darimasen");
  set_default_size(500, 330);

hackishUnhide = new DaIconModes*[20];

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

  //Gtk::Widget * image1 = Gtk::manage(new class Gtk::Arrow(Gtk::ARROW_DOWN ,Gtk::SHADOW_NONE)); 
  CompactMenu->items().push_back( Gtk::Menu_Helpers::MenuElem("\342\226\274",*m_Menu_File) );

  // glade-- told me to do it....
  menulist.push_back(Gtk::Menu_Helpers::CheckMenuElem(
    "Show Hidden",Gtk::AccelKey(GDK_H, Gdk::CONTROL_MASK), sigc::mem_fun(*this, &Darimasen::fShowHidden)));
  optShowHidden = (Gtk::CheckMenuItem *)&menulist.back();

  menulist.push_back(Gtk::Menu_Helpers::MenuElem(
    "Print history to Terminal",Gtk::AccelKey(GDK_P, Gdk::CONTROL_MASK), sigc::mem_fun(*this, &Darimasen::fPrintHist)));

  menulist.push_back( Gtk::Menu_Helpers::MenuElem("About...",
    sigc::mem_fun(*this, &Darimasen::fAbout) ) );

   // showHidden = optShowHidden->get_active();
//std:: cout << optShowHidden->get_active() << "!\n";

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

    addTab(history[i].top(), Tabber->get_n_pages());
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
/* generated by glademm-embed */

const unsigned char icon128x128_png_data[] = 
{	137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,
	0,0,0,128,0,0,0,128,8,6,0,0,0,195,62,97,
	203,0,0,0,4,115,66,73,84,8,8,8,8,124,8,100,
	136,0,0,0,25,116,69,88,116,83,111,102,116,119,97,114,
	101,0,119,119,119,46,105,110,107,115,99,97,112,101,46,111,
	114,103,155,238,60,26,0,0,19,198,73,68,65,84,120,156,
	237,157,121,112,213,213,189,192,63,223,243,251,37,225,102,133,
	172,70,214,152,0,34,129,176,132,77,64,8,2,21,91,247,
	142,58,109,165,42,195,235,179,116,145,241,189,105,157,250,250,
	124,181,181,206,188,46,35,218,169,232,76,241,213,50,237,171,
	138,62,165,84,69,141,44,74,216,100,223,100,73,98,200,66,
	72,2,132,144,133,228,222,243,254,72,114,201,205,189,73,238,
	154,92,115,127,159,153,223,228,230,252,206,242,61,231,124,127,
	223,179,254,206,79,180,214,88,68,46,106,160,5,176,24,88,
	44,5,136,112,44,5,136,112,132,28,38,210,70,212,64,11,
	98,49,32,52,11,55,80,5,100,12,180,36,22,3,194,9,
	171,9,136,112,44,5,136,108,196,82,128,8,199,82,128,8,
	199,82,128,200,198,106,2,34,29,75,1,34,27,203,2,68,
	58,150,2,68,54,150,5,136,116,44,5,136,112,44,5,136,
	100,180,213,4,68,60,38,80,6,52,3,210,197,93,186,253,
	189,246,91,186,184,105,55,127,222,197,161,61,196,229,93,28,
	225,226,127,112,32,136,88,123,2,253,71,68,122,87,152,133,
	29,191,47,119,113,107,238,248,221,210,241,183,173,203,189,206,
	223,142,46,110,246,142,223,169,30,252,219,221,252,63,6,252,
	218,135,44,20,155,62,120,182,232,134,118,125,122,6,252,73,
	146,108,105,246,53,72,216,41,128,220,32,119,162,184,3,205,
	104,132,82,160,20,7,165,104,14,233,98,189,127,160,229,27,
	108,132,149,2,72,142,220,131,176,193,249,44,117,254,21,64,
	104,144,209,50,74,151,234,11,3,36,222,160,36,172,20,0,
	152,219,203,189,120,162,88,5,252,50,212,66,72,182,204,3,
	30,4,166,34,12,67,83,3,156,214,167,245,35,161,78,187,
	95,17,154,156,195,64,201,146,33,34,189,15,11,37,95,162,
	36,87,226,67,40,82,83,199,211,222,211,245,205,16,166,13,
	128,100,203,227,34,82,40,34,171,68,228,102,65,38,136,200,
	124,17,121,88,114,100,81,168,211,15,20,17,241,254,66,26,
	76,99,156,177,70,107,189,66,76,137,37,7,187,26,171,206,
	3,87,128,120,32,10,184,10,180,2,137,130,36,1,90,141,
	85,187,181,214,207,233,83,250,173,96,10,175,68,85,244,225,
	101,156,136,136,14,209,208,69,178,101,148,82,234,247,189,120,
	121,12,248,56,20,105,7,3,101,170,88,95,186,162,90,116,
	131,9,220,39,34,177,29,110,6,112,93,31,225,4,152,169,
	68,253,4,8,170,2,8,82,209,199,72,219,70,22,163,128,
	210,96,166,219,137,50,213,157,162,165,83,24,55,4,185,91,
	114,36,71,159,210,167,66,145,126,160,40,84,130,22,239,53,
	64,68,26,148,136,24,74,20,190,94,162,164,42,232,25,80,
	170,66,148,208,219,101,68,25,11,130,157,174,51,125,81,15,
	56,211,242,108,54,77,51,202,252,99,168,210,15,20,45,58,
	209,151,38,0,168,81,74,169,94,11,188,199,75,164,46,216,
	25,48,29,102,101,95,66,43,165,150,5,59,93,128,232,9,
	209,43,69,100,158,23,237,230,226,152,27,99,190,19,10,25,
	2,69,41,149,224,75,29,42,165,62,83,136,143,29,135,206,
	138,16,229,8,118,6,154,70,52,85,137,72,115,31,105,223,
	30,155,23,59,34,152,233,218,110,180,101,137,146,223,121,91,
	112,24,172,137,185,41,38,55,152,50,4,3,17,73,240,178,
	238,218,47,135,218,166,148,168,107,55,125,208,30,20,246,96,
	103,64,23,234,54,132,55,250,72,59,81,219,245,219,182,9,
	182,49,193,72,51,118,82,236,72,109,234,127,136,72,188,15,
	15,192,48,81,242,129,109,178,237,134,96,200,16,44,148,168,
	161,94,53,223,237,121,168,110,62,222,124,66,117,51,235,190,
	92,65,183,0,0,202,84,47,247,217,118,9,211,49,57,16,
	155,27,27,208,176,48,38,47,38,87,163,119,136,200,4,63,
	172,96,38,154,205,113,55,198,93,31,172,188,7,138,136,228,
	250,208,132,111,3,80,254,152,255,142,182,48,36,10,208,116,
	160,105,155,136,28,247,66,134,68,20,175,199,230,197,254,183,
	20,248,54,165,45,249,18,27,55,57,238,41,19,115,135,82,
	106,184,82,10,63,175,27,136,225,131,164,220,164,228,80,148,
	133,47,216,242,108,89,162,36,213,219,250,67,241,38,120,82,
	0,111,53,200,144,160,55,1,78,20,175,244,210,113,113,189,
	68,253,91,220,133,184,195,241,83,227,31,31,58,121,232,176,
	94,11,105,170,109,76,252,148,248,213,113,109,113,95,136,146,
	103,124,52,251,61,93,19,237,166,253,159,105,55,166,37,132,
	172,60,188,192,192,152,239,195,195,91,219,212,216,180,1,192,
	20,229,247,18,119,72,44,0,128,25,99,254,143,190,170,159,
	5,98,188,12,50,30,248,189,221,176,63,155,48,53,97,39,
	112,74,180,156,114,224,104,21,145,20,32,21,152,109,98,78,
	14,209,138,254,204,102,91,243,190,164,233,73,223,185,180,247,
	82,81,72,82,232,3,81,178,210,219,188,137,150,63,235,147,
	186,5,192,20,241,46,148,184,199,30,50,11,80,191,179,190,
	54,97,90,194,47,69,228,25,31,131,218,128,133,29,23,170,
	127,119,188,101,107,244,246,164,105,73,191,170,79,170,127,70,
	23,234,182,254,74,56,126,90,124,158,82,106,158,215,1,20,
	47,59,127,122,219,222,121,48,197,33,179,0,0,13,251,26,
	126,165,68,189,21,4,19,221,159,151,129,226,231,137,245,137,
	159,37,206,74,28,23,202,242,233,68,238,23,195,20,243,121,
	31,100,220,86,191,167,254,120,103,120,191,59,129,72,232,154,
	0,104,223,108,17,221,28,189,92,161,142,14,112,165,218,251,
	244,227,222,63,154,161,180,218,151,52,43,233,135,146,47,33,
	61,125,37,169,52,233,215,24,44,240,97,248,254,114,215,240,
	129,40,64,232,58,129,29,84,31,174,110,112,152,142,187,69,
	201,69,191,102,43,3,185,12,217,32,134,44,26,22,63,44,
	94,68,22,138,146,127,244,50,164,242,116,197,42,212,154,161,
	198,208,83,201,179,147,87,101,21,100,13,9,102,217,72,129,
	152,195,102,13,123,90,144,127,239,158,118,47,115,0,23,146,
	227,146,223,112,137,103,232,172,161,85,130,248,126,68,140,230,
	87,117,59,235,158,10,90,142,122,33,229,230,148,219,180,67,
	191,5,4,181,16,123,224,170,32,143,213,22,213,254,169,171,
	163,220,46,49,201,117,201,155,52,218,223,37,225,42,65,126,
	19,213,20,245,82,213,129,170,43,129,8,152,54,43,109,156,
	67,28,175,1,51,125,12,250,124,109,81,237,227,93,29,92,
	103,2,125,52,141,129,100,194,23,106,63,171,125,79,27,122,
	14,194,169,16,155,251,227,130,44,232,94,249,0,122,147,110,
	49,108,198,93,74,169,207,252,156,51,184,78,148,252,166,45,
	174,173,52,109,110,218,211,195,230,12,27,227,107,57,164,205,
	75,75,72,189,57,245,89,109,232,131,162,100,166,143,86,205,
	46,34,107,187,199,25,182,125,128,238,92,248,244,194,126,209,
	50,93,148,252,81,148,92,237,45,179,126,84,206,69,37,106,
	117,221,144,186,73,181,69,181,61,14,227,170,11,171,27,108,
	216,22,139,200,198,0,148,44,5,248,79,83,153,103,210,230,
	166,109,73,159,151,190,34,101,118,74,98,111,121,23,17,149,
	126,115,250,163,192,23,34,242,164,136,196,248,145,238,79,106,
	118,212,28,115,139,59,117,110,170,191,167,132,253,252,252,246,
	243,190,14,211,130,66,202,236,148,17,134,105,252,20,120,8,
	232,181,240,250,192,174,209,175,68,71,69,255,71,121,97,121,
	141,183,129,228,126,49,210,202,211,94,70,120,52,128,180,187,
	226,160,125,143,195,113,65,142,35,156,114,104,71,138,32,57,
	64,54,48,14,72,235,91,176,30,221,255,183,122,107,245,131,
	158,110,121,61,15,224,33,210,126,107,2,186,83,91,84,123,
	22,248,129,228,203,234,244,33,233,11,17,238,210,232,59,5,
	25,217,107,192,246,172,54,2,59,128,173,202,161,222,172,220,
	94,121,196,215,244,245,223,181,29,88,145,113,75,70,57,240,
	20,61,23,189,183,40,32,171,227,90,6,160,130,247,210,214,
	65,177,201,138,158,110,74,250,188,244,42,132,12,241,53,15,
	194,147,85,91,170,158,11,84,186,96,50,252,150,225,211,236,
	98,159,143,16,135,198,38,200,16,7,14,155,32,67,4,57,
	233,16,199,214,234,134,234,61,122,143,110,13,86,154,25,11,
	51,150,136,150,87,129,176,89,20,234,194,110,67,25,183,247,
	102,221,76,101,248,173,105,253,218,7,240,134,242,173,229,159,
	3,159,247,103,154,231,62,57,183,121,196,226,17,147,219,218,
	218,94,17,228,158,160,39,224,191,109,121,95,71,235,251,202,
	223,47,239,117,196,225,119,19,160,209,3,214,4,132,27,103,
	63,60,91,11,220,155,89,144,249,176,136,252,154,190,247,85,
	134,154,87,42,235,43,87,121,99,233,252,94,13,52,196,8,
	59,11,48,208,84,22,86,190,234,136,114,228,136,200,127,137,
	200,149,80,13,87,123,153,232,57,102,24,70,65,197,199,21,
	255,226,109,51,167,220,42,215,219,97,160,17,234,226,252,106,
	82,245,126,213,149,242,143,202,159,142,118,68,231,136,146,151,
	68,164,201,165,242,252,223,123,208,227,186,140,40,105,194,224,
	103,67,83,134,78,41,219,92,246,137,47,242,154,136,199,149,
	62,239,194,90,244,72,113,97,113,21,240,88,86,65,214,147,
	109,102,219,67,34,178,18,152,20,236,116,4,217,36,34,63,
	40,121,191,164,216,159,240,166,82,254,117,2,37,12,95,44,
	13,71,138,11,139,47,2,47,0,47,100,45,201,154,237,48,
	28,15,138,150,2,141,158,68,96,195,199,147,34,242,100,201,
	123,37,111,6,34,159,223,157,64,75,1,124,167,120,115,113,
	17,80,4,48,98,241,136,20,51,202,92,0,44,68,147,131,
	48,28,24,1,116,223,94,118,5,168,7,78,211,62,127,81,
	132,102,71,201,123,37,149,193,144,169,125,71,144,151,47,147,
	184,52,21,218,106,2,2,161,99,228,176,161,227,114,50,242,
	254,145,54,154,72,54,77,243,74,73,84,201,229,142,73,167,
	144,97,42,81,254,25,34,177,20,32,20,148,253,189,172,9,
	40,239,175,244,252,159,10,182,44,192,160,192,255,137,32,209,
	150,2,12,2,252,223,21,108,89,128,176,96,228,253,35,109,
	177,205,177,95,3,124,159,155,17,138,253,182,0,74,148,165,
	0,97,64,124,91,124,172,195,112,248,245,154,190,32,107,253,
	239,3,4,182,14,111,17,36,174,187,112,221,165,202,97,254,
	143,8,21,10,255,54,77,138,100,6,49,31,22,126,82,88,
	88,216,38,74,26,253,172,195,142,153,64,63,14,92,17,145,
	112,92,255,142,72,68,228,18,16,219,167,199,110,104,209,152,
	210,190,24,208,53,50,239,18,197,82,128,112,161,67,1,124,
	182,200,29,139,83,221,86,154,188,223,20,154,148,127,103,190,
	207,90,103,17,124,68,73,147,63,77,0,10,29,72,39,144,
	150,216,150,76,218,231,168,45,6,16,17,25,238,87,56,45,
	226,89,1,188,127,203,244,122,44,5,24,80,242,150,231,197,
	41,81,233,254,132,109,239,3,248,59,17,4,8,214,72,96,
	160,81,14,53,69,43,255,142,77,20,36,128,181,0,172,145,
	64,88,32,44,247,115,67,79,135,2,4,96,1,208,97,185,
	21,58,98,200,255,94,126,172,136,120,124,225,163,79,58,207,
	195,156,250,221,169,85,104,191,222,12,130,246,67,34,252,58,
	8,193,95,173,13,91,6,38,59,2,68,7,16,122,173,9,
	222,143,253,175,133,115,250,55,240,118,9,98,144,213,247,96,
	64,163,49,13,101,109,239,29,20,248,241,128,137,14,176,19,
	56,232,25,228,69,35,50,16,10,48,200,11,53,220,232,181,
	175,37,129,29,19,103,17,98,252,234,40,251,16,68,163,35,
	176,9,240,171,76,7,103,25,5,167,15,16,230,101,51,88,
	43,47,24,180,247,1,12,239,223,11,232,111,172,202,235,32,
	132,197,224,182,31,32,88,244,91,229,89,58,18,16,30,155,
	128,136,235,23,68,40,90,107,255,95,14,181,8,128,48,121,
	190,6,102,30,32,84,12,146,108,244,39,129,175,6,90,12,
	56,129,246,181,6,143,5,24,96,190,138,229,24,254,19,65,
	253,54,144,8,227,50,8,33,97,63,15,224,13,145,90,121,
	193,160,93,1,66,52,15,224,149,0,225,94,121,97,46,94,
	192,232,192,142,136,241,39,144,69,56,33,1,28,18,101,17,
	166,248,186,26,168,68,161,191,202,157,128,80,51,136,173,150,
	32,1,157,19,216,25,139,69,152,211,99,253,106,63,55,132,
	100,103,102,51,41,107,82,215,239,208,210,216,220,200,197,43,
	23,57,82,122,132,75,87,46,121,22,68,132,101,249,203,72,
	77,74,197,238,176,83,94,83,206,39,7,63,113,241,163,148,
	226,222,185,247,186,184,181,218,91,169,173,175,165,230,82,13,
	181,245,181,212,94,174,197,225,184,118,82,237,156,9,115,24,
	59,124,44,109,246,54,14,156,57,192,145,210,35,68,153,81,
	220,53,231,46,151,120,142,150,30,229,232,151,71,1,24,26,
	55,148,197,211,22,187,220,255,252,212,231,156,169,60,227,226,
	150,20,151,196,109,249,183,17,19,21,67,107,91,43,123,78,
	238,225,100,249,73,143,249,27,158,58,156,57,19,230,144,154,
	152,138,97,24,28,251,242,24,123,79,238,117,41,143,228,132,
	100,22,77,233,253,171,51,205,87,155,217,184,115,35,0,243,
	114,231,145,49,52,3,135,118,176,247,228,94,202,206,151,57,
	253,165,38,166,146,63,46,159,132,216,4,78,150,159,100,255,
	233,253,189,198,235,134,248,217,9,204,205,202,101,229,178,149,
	30,239,105,173,217,119,122,31,207,254,237,89,154,175,54,187,
	220,91,54,99,25,171,238,88,229,252,255,106,235,85,118,127,
	177,155,198,150,70,167,155,105,152,60,178,244,145,94,211,63,
	119,225,28,171,215,174,166,190,177,30,128,149,183,175,36,99,
	104,251,206,118,17,225,232,151,71,137,137,138,113,139,167,232,
	88,17,199,254,122,12,4,242,199,231,187,221,111,108,105,164,
	248,156,235,129,155,79,124,243,9,166,231,76,119,254,63,126,
	228,120,126,177,254,23,110,50,221,55,239,62,30,94,242,176,
	139,219,237,51,110,71,107,205,230,207,55,243,226,187,47,162,
	181,38,99,88,70,159,249,179,59,236,108,218,189,9,17,225,
	137,251,158,32,218,108,223,249,189,100,218,18,86,175,93,77,
	107,91,251,49,192,15,221,250,16,183,78,189,21,128,127,238,
	254,39,7,206,28,232,53,222,238,104,116,15,159,140,241,226,
	96,129,158,16,17,166,229,76,227,71,119,255,200,45,220,215,
	166,127,205,197,111,116,84,52,243,39,205,119,127,107,181,15,
	50,134,101,176,114,217,74,167,255,206,2,106,23,128,30,227,
	201,29,147,139,97,24,40,81,228,101,229,121,148,189,235,225,
	203,233,67,211,153,150,61,205,197,207,244,156,233,36,39,36,
	187,148,87,214,117,89,44,191,117,57,0,151,155,46,243,250,
	182,215,217,243,197,30,180,214,136,8,75,167,47,101,193,164,
	5,157,111,85,187,113,225,242,5,170,47,86,83,83,95,67,
	99,115,35,39,206,158,104,31,163,27,166,75,222,70,167,143,
	230,219,5,223,238,250,134,182,107,190,125,61,116,90,169,208,
	173,5,220,146,123,11,155,118,111,114,154,220,209,233,163,201,
	185,62,199,205,223,226,169,139,217,188,111,115,151,124,120,39,
	207,232,140,209,30,191,170,33,180,87,162,167,120,226,109,241,
	100,103,102,115,186,242,52,147,111,152,236,49,222,174,202,189,
	40,111,145,155,178,43,165,40,200,43,224,173,207,174,29,203,
	51,110,196,56,58,71,83,13,77,13,188,187,243,93,46,93,
	185,196,207,30,252,25,163,210,71,81,91,95,75,188,45,190,
	189,224,187,201,245,250,182,215,249,75,225,95,220,229,80,158,
	149,229,158,185,247,176,235,139,93,28,63,123,220,237,92,7,
	95,235,210,239,169,96,111,195,76,31,59,157,227,101,237,31,
	169,92,60,117,177,71,63,19,70,78,96,68,202,8,42,234,
	42,0,239,63,149,114,254,226,121,143,114,116,181,96,158,152,
	148,53,137,198,171,141,164,38,166,246,24,182,147,158,218,234,
	69,83,22,241,118,209,219,206,255,47,55,93,118,254,206,76,
	206,228,133,199,94,224,96,241,65,182,28,222,194,190,211,251,
	156,77,156,39,185,102,142,159,201,137,242,19,52,181,52,17,
	59,36,150,184,33,113,28,42,57,68,109,125,45,90,187,143,
	206,148,40,126,124,247,143,121,124,237,227,110,202,228,243,11,
	62,254,238,9,244,246,41,205,201,204,1,1,67,25,44,156,
	180,176,71,127,5,83,10,88,95,184,190,51,242,62,177,59,
	236,188,179,235,29,159,70,32,118,135,29,67,25,76,206,154,
	76,211,213,38,55,247,238,76,24,57,129,235,147,61,191,250,
	56,42,109,20,57,153,57,156,170,60,5,192,206,19,59,217,
	126,100,59,243,38,182,127,190,55,41,46,137,249,185,243,153,
	159,59,31,187,195,206,230,125,155,89,187,105,173,199,225,246,
	232,244,209,60,245,160,235,231,23,223,252,244,77,94,251,248,
	53,55,255,151,155,46,147,96,75,224,250,228,235,157,77,78,
	160,132,116,22,168,179,160,243,199,230,147,20,151,212,163,191,
	130,201,5,61,106,111,77,189,235,231,110,174,182,93,229,167,
	235,126,202,145,82,239,191,245,212,210,218,194,246,35,219,1,
	184,105,212,77,76,185,97,10,0,13,205,13,20,29,247,252,
	149,184,69,121,189,247,212,187,91,135,223,110,248,45,207,255,
	223,243,236,63,179,159,171,109,87,157,238,134,50,184,109,250,
	109,124,119,241,119,61,198,163,209,92,104,184,192,185,139,231,
	168,186,80,69,69,93,5,231,46,158,243,232,119,221,230,117,
	206,184,191,62,243,235,140,31,49,190,87,25,189,33,164,103,
	253,117,14,127,250,42,204,212,196,84,38,143,153,204,129,98,
	247,94,236,206,19,59,209,90,243,141,153,223,0,32,218,140,
	38,59,51,155,147,21,158,135,98,158,56,119,241,28,31,237,
	255,136,5,147,22,96,139,182,49,107,252,44,0,10,15,22,
	210,212,210,228,230,63,218,140,102,238,77,115,123,141,243,150,
	137,183,176,238,131,117,180,218,91,137,54,163,73,78,72,230,
	116,229,105,62,57,244,9,49,102,12,139,167,46,102,249,173,
	203,157,157,184,25,227,102,240,234,135,175,186,197,179,190,112,
	61,111,108,127,195,205,221,19,103,107,206,242,218,199,175,177,
	98,233,10,4,97,120,138,95,7,131,184,16,82,11,240,233,
	209,79,73,138,75,34,127,108,126,159,126,123,82,146,198,150,
	70,214,109,94,199,177,178,107,223,60,124,116,233,163,140,201,
	24,227,181,28,117,151,235,56,84,114,200,105,77,58,77,254,
	246,195,219,221,44,12,180,207,43,196,198,244,126,252,81,188,
	45,158,25,227,102,0,240,187,149,191,227,165,31,188,196,154,
	127,93,195,3,243,31,160,185,181,153,141,187,54,186,116,20,
	19,108,9,128,123,243,217,117,92,239,13,27,119,110,228,112,
	233,97,159,194,244,70,200,20,160,228,92,9,135,75,15,179,
	112,210,66,151,54,182,161,169,129,19,103,79,184,77,22,205,
	190,113,54,49,81,49,110,241,52,181,52,97,119,216,249,195,
	198,63,224,208,237,147,63,209,102,52,43,111,243,60,15,225,
	9,17,65,163,57,88,124,208,233,214,220,218,204,169,202,83,
	46,19,74,157,205,80,119,101,172,168,173,160,228,92,137,219,
	188,70,167,191,93,95,236,114,186,61,112,203,3,172,88,186,
	130,123,111,190,151,165,211,150,58,221,183,29,217,6,224,214,
	174,127,255,235,223,231,229,31,190,236,114,173,249,222,26,82,
	18,83,60,230,69,163,89,243,206,26,151,126,76,32,132,164,
	9,216,113,124,7,47,190,251,34,224,90,152,251,207,236,231,
	153,191,62,131,221,209,126,4,254,248,225,227,121,238,145,231,
	16,17,98,162,98,200,31,155,207,174,19,187,92,226,186,210,
	220,254,213,179,179,53,103,249,104,255,71,44,153,186,4,128,
	137,163,38,50,61,103,58,123,79,237,117,241,223,169,36,93,
	233,28,89,28,43,59,230,148,231,120,217,113,236,14,187,75,
	223,35,202,136,34,45,41,141,201,99,174,13,17,95,253,240,
	85,222,222,113,173,199,127,255,252,251,249,214,194,111,1,48,
	45,103,26,105,73,105,172,47,92,79,118,102,54,121,89,121,
	136,8,119,204,186,195,37,253,163,95,30,237,209,204,247,212,
	55,74,140,77,164,182,190,214,227,189,234,139,213,252,233,131,
	63,177,234,27,171,60,222,247,5,191,20,160,172,166,140,194,
	131,133,206,130,85,162,104,181,183,82,90,93,202,201,138,147,
	206,177,127,98,108,34,165,213,165,84,212,85,112,165,249,10,
	127,254,232,207,206,202,7,56,81,126,130,231,223,121,158,113,
	195,199,145,20,155,68,75,107,11,14,237,96,235,225,173,196,
	68,197,96,119,216,57,84,114,200,233,255,111,91,254,70,180,
	25,141,105,152,40,165,176,197,216,0,40,58,94,196,176,248,
	97,0,78,133,176,59,236,108,59,178,13,91,180,141,157,39,
	118,2,237,10,184,245,240,86,12,101,240,222,222,247,0,168,
	172,171,228,195,125,31,210,106,111,101,215,201,93,100,38,103,
	242,233,209,79,49,148,193,153,170,51,46,149,15,176,225,179,
	13,104,173,73,75,74,195,161,29,196,13,137,227,252,165,243,
	60,253,151,167,153,59,113,46,179,199,207,38,51,57,147,234,
	75,213,148,157,47,163,180,186,148,29,199,118,56,21,243,82,
	227,37,62,62,240,113,159,101,92,119,185,14,128,45,135,182,
	16,101,70,81,92,85,76,69,109,133,243,254,230,125,155,73,
	75,74,35,222,22,79,75,107,139,179,204,125,69,238,250,197,
	93,254,126,59,216,226,171,142,102,109,216,109,6,112,153,214,
	237,103,60,245,65,6,59,97,167,0,93,199,208,253,77,75,
	107,203,128,165,61,80,132,157,2,88,244,47,150,2,68,56,
	150,2,68,56,150,2,68,56,150,2,68,56,150,2,68,56,
	150,2,68,56,150,2,68,56,150,2,68,56,150,2,68,56,
	150,2,68,56,150,2,68,56,150,2,68,56,150,2,68,56,
	150,2,68,56,150,2,68,56,150,2,68,56,166,3,199,173,
	134,195,136,26,104,65,44,250,31,133,170,17,79,47,32,90,
	68,14,86,19,16,225,88,10,16,225,88,10,16,225,252,63,
	9,207,200,6,170,34,217,246,0,0,0,0,73,69,78,68,
	174,66,96,130,
};
const unsigned icon128x128_png_size = 5172;






    Glib::RefPtr<Gdk::PixbufLoader> loader=Gdk::PixbufLoader::create();
    loader->write(icon128x128_png_data, icon128x128_png_size);
    loader->close();



   Gtk::AboutDialog aboutDarimasen;





  aboutDarimasen.set_logo (loader->get_pixbuf());
  aboutDarimasen.set_name ("Darimasen");
  aboutDarimasen.set_version ("0.0.7");
  aboutDarimasen.set_website ("http://darimasen.berlios.de/");

  aboutDarimasen.run();
  }

/**********************/

void Darimasen::fShowHidden(){


  DarimasenMenuContainer->remove();
  DaMenu = Gtk::manage( new DarimasenMenu(history[Tabber->get_current_page()].top(), *this));
  DarimasenMenuContainer->add(*DaMenu);



  for (int i = 0; i < Tabber->get_n_pages(); i++){
hackishUnhide[i]->SwitchHidden();
    }
std::cout << "\n";

  }

/**********************/

void Darimasen::fBack(){
  history[Tabber->get_current_page()].pop();
  Glib::ustring tmp = history[Tabber->get_current_page()].top();
  history[Tabber->get_current_page()].pop();
  ChangeCurrentPath(tmp);
  //std::cout << tmp <<"\n";

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
