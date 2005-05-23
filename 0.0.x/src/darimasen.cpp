/* Darimasen - darimasen.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

/**********************/

#include "darimasen.h"

/**********************/

Darimasen::Darimasen(std::vector<Glib::ustring> paths){
  set_title("Darimasen");
  set_default_size(500, 330);

  mode = 0;

  try{
     windowIcon = Gdk::Pixbuf::create_from_file("/usr/share/icons/hicolor/48x48/apps/darimasen.png");
    set_icon(windowIcon);
    }
  catch(const Glib::Error) {
    try{
      windowIcon = Gdk::Pixbuf::create_from_file("../pixmaps/48x48/darimasen.png");
      set_icon(windowIcon);
      }
    catch(const Glib::Error) {}
    }

  add(VerticalOrganizer);
  VerticalOrganizer.show();

  VerticalOrganizer.pack_start(TopBar, Gtk::PACK_SHRINK);
  TopBar.show();

  TopBar.set_toolbar_style(Gtk::TOOLBAR_ICONS); 


  CompactMenuContainer = new Gtk::ToolItem;
  TopBar.append(*CompactMenuContainer);
  CompactMenuContainer->show();

  CompactMenu = new Gtk::MenuBar;
  m_Menu_File = new Gtk::Menu;



  CompactMenu->items().push_back(Gtk::Menu_Helpers::MenuElem(
    "\342\226\274", *m_Menu_File));

  // glade-- told me to do it....
  m_Menu_File->items().push_back(Gtk::Menu_Helpers::CheckMenuElem(
    "Show Hidden",Gtk::AccelKey(GDK_H, Gdk::CONTROL_MASK),
      sigc::mem_fun(*this, &Darimasen::fShowHidden)));

  optShowHidden = (Gtk::CheckMenuItem *)&m_Menu_File->items().back();

  m_Menu_File->items().push_back(Gtk::Menu_Helpers::MenuElem(
    "Print history to Terminal",Gtk::AccelKey(GDK_P, Gdk::CONTROL_MASK),
      sigc::mem_fun(*this, &Darimasen::fPrintHist)));

  m_Menu_File->items().push_back( Gtk::Menu_Helpers::MenuElem("About...",
    sigc::mem_fun(*this, &Darimasen::fAbout) ) );

  m_Menu_File->items().push_back( Gtk::Menu_Helpers::SeparatorElem() ) ;

  m_Menu_File->items().push_back( Gtk::Menu_Helpers::StockMenuElem(Gtk::Stock::QUIT,
    sigc::mem_fun(*this, &Darimasen::fQuit) ) );
    
  TopBar.set_show_arrow(false);
  CompactMenuContainer->add(*CompactMenu); 
  CompactMenu->show();


  Gtk::SeparatorToolItem * sep1 = new Gtk::SeparatorToolItem;
  TopBar.append(*sep1);
  sep1->show();

  DarimasenMenuContainer = new Gtk::ToolItem;
  TopBar.append(*DarimasenMenuContainer);
  DarimasenMenuContainer->set_expand(true);
  DarimasenMenuContainer->show();

  Gtk::SeparatorToolItem * sep2  = new Gtk::SeparatorToolItem;
  TopBar.append(*sep2);
  sep2->show();

  BackButton = new Gtk::MenuToolButton(Gtk::StockID("gtk-go-back"));
  BackButton->signal_clicked().connect(sigc::mem_fun(*this, &Darimasen::fBack));
  TopBar.append(*BackButton);
  BackButton->set_sensitive(false);
  BackButton->show();
  
  ChangeIconMode = new Gtk::ToolButton(Gtk::StockID("gtk-convert"));
  ChangeIconMode->signal_clicked().connect(sigc::mem_fun(*this, &Darimasen::fChangeIconMode));
  TopBar.append(*ChangeIconMode);
  //ChangeIconMode->set_sensitive(false);
  ChangeIconMode->show();
   
  //ViewTree = new Gtk::ToggleToolButton(Gtk::StockID("gtk-index"));
  //ViewTree->signal_clicked().connect(sigc::mem_fun(*this, &Darimasen::fViewTree));
  //TopBar.append(*ViewTree);
  //ViewTree->set_sensitive(false);
  //ViewTree -> show();

  NewTab = new Gtk::ToolButton(Gtk::StockID("gtk-new"));
  NewTab->signal_clicked().connect(sigc::mem_fun(*this, &Darimasen::fNewTab));
  TopBar.append(*NewTab);
  NewTab->show();

  VerticalOrganizer.add(HideTreePane);
  HideTreePane.show();

  HideTreePane.pack1(TreeScroller, Gtk::SHRINK);

  Tabber = new Gtk::Notebook;
  Tabber->signal_switch_page().connect(sigc::mem_fun(*this, &Darimasen::tabberSwitched));
  Tabber->set_show_border(false);
  Tabber->show();

  HideTreePane.pack2(*Tabber, Gtk::EXPAND|Gtk::SHRINK);
   
  Info.set_has_resize_grip();
  Info.show();
  VerticalOrganizer.pack_start(Info, Gtk::PACK_SHRINK);

  show();

  for(int i = 0; i < paths.size(); i++){
    set_history(i,paths[i]);
    addTab(Tabber->get_n_pages());
    }


  myOptions = new optionsQuery(Glib::get_home_dir() + get_choices_dir() + "/Darimasen/Options");
 }

/**********************/

Darimasen::~Darimasen(){
  fQuit();
  }

/**********************/

// all the stuff for a well rounded tab
void Darimasen::addTab(guint pos){

  if( Tabber->get_n_pages() == 0 )
    Tabber->set_show_tabs(false);
  else
    Tabber->set_show_tabs(true);


  Gtk::Image * xed = 
    new Gtk::Image("/usr/share/icons/hicolor/16x16/stock/generic/stock_close.png");
  xed->show();

  Gtk::Label * tabNum;

  if ( get_history(pos) == slash)
    tabNum = new Gtk::Label(slash + " ");
  else if ( get_history(pos) == (Glib::get_home_dir() + slash))
    tabNum = new Gtk::Label("~ ");
  else
    tabNum = new Gtk::Label(
      get_history(pos).substr(get_history(pos).rfind(slash,get_history(pos).length() - 2  ) + 1));


  Gtk::HBox * arrangement= new Gtk::HBox() ;
  Gtk::Button * closeButton = new Gtk::Button();

  closeButton->add(*xed);
  closeButton->set_relief(Gtk::RELIEF_NONE); 
  closeButton->show();




  tabNum->show();
  arrangement->show();

  arrangement->pack_start(*tabNum);
  arrangement->pack_end(*closeButton);

  Gtk::EventBox * MainEventBox = new class Gtk::EventBox;


  MainEventBox->modify_bg(Gtk::STATE_NORMAL, Gdk::Color("#ffffff"));
  MainEventBox->show();

  DaIconModes * foo;
  foo = new class DaIconModes(pos, *this);


  // pos can't be the thing passed here, it changes... (segfault: 3 tabs, close 2nd then 3rd)
  closeButton->signal_clicked().connect(
    sigc::bind<DaIconModes*>(sigc::mem_fun(*this, &Darimasen::removeTab), foo) );



  if ( IconModeList.size() == pos ){
    IconModeList.push_back(foo);
    EventBoxList.push_back(MainEventBox);
    }
  else {
    delete IconModeList[pos];
    IconModeList[pos] = foo;
    delete EventBoxList[pos];
    EventBoxList[pos] = MainEventBox;
    }

  Gtk::ScrolledWindow * MainScroller = new Gtk::ScrolledWindow;
  MainScroller->show();
  MainScroller->set_shadow_type(Gtk::SHADOW_NONE);
  MainScroller->add(*MainEventBox);
  MainEventBox->add(*foo);
  MainScroller->set_policy(Gtk::POLICY_ALWAYS, Gtk::POLICY_AUTOMATIC);
  foo->show();
  Tabber->insert_page( *MainScroller, *arrangement, pos);
  }

/**********************/

void Darimasen::fAbout(){

  about = new Gtk::Window();

  Gtk::Image * image1;
  try{
    image1 = new class Gtk::Image(  Gdk::Pixbuf::create_from_file( 
      "/usr/share/pixmaps/darimasenInfo.svg" ) );
    }
  catch(const Glib::Error) {
    try{
    image1 = new class Gtk::Image(  Gdk::Pixbuf::create_from_file( 
      "../pixmaps/darimasenInfo.svg" ) );
      }
  catch(const Glib::Error) {}
    }


  Gtk::EventBox * closer = new Gtk::EventBox;
  closer->add(*image1);
  closer->signal_button_press_event().connect(
    sigc::mem_fun(*this, &Darimasen::fAbout_ch0wned));

  about->add(*closer);
  about->set_title("About Darimasen");
  about->set_modal(true);
  about->set_resizable(false);
  about->set_decorated (false);
  about->set_position(Gtk::WIN_POS_CENTER);

  about->show_all_children();


  Gtk::Main::run(*about); 

  delete image1;
  delete closer;
  delete about;
  }

/**********************/

bool Darimasen::fAbout_ch0wned(GdkEventButton*){
  about->hide();
  }

/**********************/

void Darimasen::fBack(){
  del_history(Tabber->get_current_page());

  ChangeCurrentPath(get_history(Tabber->get_current_page()),false,false);

  if (history[Tabber->get_current_page()].size() == 1)
    BackButton->set_sensitive(false);
  }

/**********************/

void Darimasen::fChangeIconMode(){
  mode = (mode + 1) % 3; // increment, mod of possibilities.
  for(int i = 0; i < history.size(); i++){
      ChangeCurrentPath(get_history(i),false,false);
    } 
  }

/**********************/

// anything prefixed with 'f' corresponds with a button. 'nuff said.
void Darimasen::fNewTab(){
  set_history(Tabber->get_n_pages(), Glib::get_home_dir() + slash);
  addTab(Tabber->get_n_pages());
  }

/**********************/

void Darimasen::fPrintHist(){
  std::vector< std::vector<Glib::ustring> > destroy = history;
  
  std::cout << "There are " << destroy.size() << " tabs here.\n";
  
  for (int i = 0; i < destroy.size(); i++){
    for (; destroy[i].size() > 0; destroy[i].pop_back()){
      std::cout << "history[" << i << "].end() = " << destroy[i][destroy[i].size() -1] << "\n";
      }
    }
  }

/**********************/

void Darimasen::fQuit(){

  delete Tabber;

  delete BackButton;
  delete ChangeIconMode;
  //delete ViewTree;
  delete NewTab;
  DarimasenMenuContainer->remove();

  delete DarimasenMenuContainer;
  delete m_Menu_File;
  delete CompactMenu;
  delete CompactMenuContainer;

  windowIcon.clear();
  for (int i=0; i < IconModeList.size(); i++){
     delete IconModeList[i];
     delete EventBoxList[i];
     }

  delete myOptions;

  hide();
  }

/**********************/

void Darimasen::fShowHidden(){
  DarimasenMenuContainer->remove();
  DaMenu = Gtk::manage( new DarimasenMenu(
    get_history(Tabber->get_current_page()), *this, Tabber->get_current_page()));
  DarimasenMenuContainer->add(*DaMenu);

  for( int i = 0; IconModeList.size() > i; i++)
    IconModeList[i]->SwitchHidden(i);
  }

/**********************/

// removing a tab has some cleanup involved...
void Darimasen::removeTab(DaIconModes*& toRemove){

  guint pos = toRemove->get_position();

  if( Tabber->get_n_pages() == 2 )
    Tabber->set_show_tabs(false);
  else
    Tabber->set_show_tabs(true);

  guint tmp = pos;
  Tabber->remove_page(pos);
  
  while ( del_history(pos) > 0 );

  delete IconModeList[pos];
  IconModeList.erase(IconModeList.begin()+pos,IconModeList.begin()+pos+1 );

  delete EventBoxList[pos];
  EventBoxList.erase(EventBoxList.begin()+pos,EventBoxList.begin()+pos+1 );

  for(guint i = 0; i < Tabber->get_n_pages(); i++){
    IconModeList[i]->update_position(i);
    std::cout << i << "\n";
    }
  }

/**********************/

// this is for a signal handler.
void Darimasen::tabberSwitched(GtkNotebookPage* sig, guint n){


  DarimasenMenuContainer->remove();
  DaMenu = Gtk::manage(new class DarimasenMenu(get_history(n), *this, n));
  DarimasenMenuContainer->add(*DaMenu);

  if (history[n].size() == 1)
    BackButton->set_sensitive(false);
  else
    BackButton->set_sensitive(true);

  buildHistoryMenu(n);
  }

/**********************/

void Darimasen::buildHistoryMenu(gint tab){
  if(BackButton->get_menu() != NULL){
    delete BackButton->get_menu();
    }

  history_menu = new Gtk::Menu();
  BackButton->set_menu(*history_menu);

  for( int i = 1; i <  history[tab].size(); i++){
    history_menu->items().push_back(Gtk::Menu_Helpers::MenuElem(
      get_history(tab,i),sigc::bind<gint,gint>(sigc::mem_fun(*this, &Darimasen::fHistoryMenu), tab, i)));
    }
  }

/**********************/

// changing directories actually is making a new tab to substitute for the old one.
void Darimasen::ChangeCurrentPath(Glib::ustring pathin, bool addPath, bool menuOnly){
  guint nth = Tabber->get_current_page();

  if (addPath && !menuOnly){
    if (pathin.substr(pathin.length()-1) != "/")
      set_history(nth, pathin + slash);
    else
      set_history(nth, pathin);
    }

  if  (!menuOnly){
    if ( history[nth].size() > 1)
      BackButton->set_sensitive(true);
    else
      BackButton->set_sensitive(false);

      Tabber->remove_page(nth);
      addTab(nth);
      Tabber->set_current_page(nth);
    }

  DarimasenMenuContainer->remove();
  DaMenu = Gtk::manage( new DarimasenMenu( get_history(nth), *this,nth));
  DarimasenMenuContainer->add(*DaMenu);

  }

/**********************/

bool Darimasen::del_history(gint tab){
  history[tab].pop_back();
  if ( history[tab].size() == 0 ){
    history.erase(history.begin()+tab,history.begin()+tab+1 );
    return 0;
    }
  return history[tab].size();
  }

/**********************/

bool Darimasen::get_hidden(){
  return optShowHidden->get_active();
  }

/**********************/

Glib::ustring Darimasen::get_history(gint tab, gint level){
  return history[tab][history[tab].size()-level-1];
  }

/**********************/

Glib::RefPtr<Gdk::Pixbuf> Darimasen::get_icon(Glib::ustring mime){
  int i;

  for(i = 0; i< mimeList.size(); i++){
    if( mime == mimeList[i] ){
      return unsizedImg[i];
      }
    }
   // pic doesn't exist.

    Glib::ustring ico;
  try{
    ico = "/usr/share/icons/gnome/48x48/mimetypes/gnome-mime-";
    ico += mime.replace(mime.find("/"), 1, "-");
    ico += ".png";

    Glib::file_get_contents(ico);
    }
  catch(const Glib::Error) {
    try{
      ico = "/usr/share/icons/gnome/48x48/mimetypes/gnome-mime-";
      ico += mime.substr(0,mime.find("-"));
      ico += ".png";
      Glib::file_get_contents(ico);
      }
    catch(const Glib::Error) {
     ico = "/usr/share/icons/gnome/48x48/filesystems/gnome-fs-loading-icon.png";
     }
    }

  mimeList.push_back(mime);
  Glib::RefPtr<Gdk::Pixbuf> xe = Gdk::Pixbuf::create_from_file(ico);
  unsizedImg.push_back(xe);
  return unsizedImg[i]; 
  }

/**********************/

short Darimasen::get_mode(){
  return mode;
  }

/**********************/

void Darimasen::fHistoryMenu(gint tab, gint iterations){
  for(int i = 0; i < iterations; i++)
    del_history(Tabber->get_current_page());

  ChangeCurrentPath(get_history(Tabber->get_current_page()),false,false);

  if (history[Tabber->get_current_page()].size() == 1)
    BackButton->set_sensitive(false);
  }

/**********************/

// ...and there was already one with an f.
void Darimasen::newTab(Glib::ustring newpath){
  if( newpath.substr(newpath.length() - 1) != slash)
    newpath += slash;

  set_history(Tabber->get_n_pages(), newpath);
  addTab(Tabber->get_n_pages());
  }

/**********************/

void Darimasen::set_message(Glib::ustring in){
  Info.pop();
  Info.push(in);
  }

/**********************/

void Darimasen::set_history(gint tab, Glib::ustring path){
  if (Tabber->get_n_pages() == tab){
    std::vector<Glib::ustring> empty;
    history.push_back(empty);
    history[tab].push_back(path);
    }
  else {
    history[tab].push_back(path);
    }
  }

/**********************/

// this is called on file operations - since there is more then one tab,
// all of them should be checked to see if they should be updated.
void Darimasen::updateView(Glib::ustring sourceDir, Glib::ustring targetDir){

  for(int i = 0; i < history.size(); i++){
    if( get_history(i) == sourceDir || get_history(i) == targetDir ){

/*
if (!Gnome::Vfs::Uri::create(get_history(i))->uri_exists()){
  Glib::ustring temp = get_history(i);
  del_history(i);
  set_history(i, temp.substr(0, temp.rfind(slash)) + slash);
}*/

      ChangeCurrentPath(get_history(i),false,false);
      }
    }
  }

/**********************/
