/* Darimasen - darimasen.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

/**********************/

#include "darimasen.h"
#include <iostream>


/**********************/

// anything prefixed with 'f' corresponds with a button. 'nuff said.
void Darimasen::fNewTab(){
  set_history(Tabber->get_n_pages(), getenv("HOME") + slash);
  addTab(Tabber->get_n_pages());

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

// all the stuff for a well rounded tab
void Darimasen::addTab(guint pos){

  if( Tabber->get_n_pages() == 0 )
    Tabber->set_show_tabs(false);
  else
    Tabber->set_show_tabs(true);


  Gtk::Image * xed = Gtk::manage(
    new Gtk::Image("/usr/share/icons/hicolor/16x16/stock/generic/stock_close.png"));
  xed->show();

  Gtk::Label * tabNum;

  if ( get_history(pos) == slash)
    tabNum = Gtk::manage(new Gtk::Label(slash + " "));
  else if ( get_history(pos) == (getenv("HOME") + slash))
    tabNum = Gtk::manage(new Gtk::Label("~ "));
  else
    tabNum = Gtk::manage(new Gtk::Label(
      get_history(pos).substr(get_history(pos).rfind(slash,get_history(pos).length() - 2  ) + 1)));


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

  Gtk::EventBox * MainEventBox = new class Gtk::EventBox;

  Gdk::Color m_Color;
  m_Color.set_rgb(65535, 65535, 65535);
  MainEventBox->modify_bg(Gtk::STATE_NORMAL, m_Color);
  MainEventBox->show();

  DaIconModes * foo;
  foo = new DaIconModes(  pos, *this);

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

 // Gtk::ScrolledWindow * MainScroller = Gtk::manage(new Gtk::ScrolledWindow);
 // MainScroller->show();
 // MainScroller->set_shadow_type(Gtk::SHADOW_NONE);
 // MainScroller->add(*MainEventBox);
  MainEventBox->add(*foo);
 // MainScroller->set_policy(Gtk::POLICY_ALWAYS, Gtk::POLICY_ALWAYS);
  foo->show();
  Tabber->insert_page( *MainEventBox, *arrangement, pos);

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

// removing a tab has some cleanup involved...
void Darimasen::removeTab(guint pos){

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
  }


/**********************/

Darimasen::Darimasen(std::vector<Glib::ustring> paths){
  set_title("Darimasen");
  set_default_size(500, 330);

  mode = 0;

  try{
    set_icon_from_file( (Glib::ustring)DATADIR + (Glib::ustring)"/icons/hicolor/48x48/apps/darimasen.png"  );
  }
  catch(const Glib::Error) {/*non-existant file OR folder*/}
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



  CompactMenu->items().push_back( Gtk::Menu_Helpers::MenuElem("\342\226\274", *m_Menu_File) );

  // glade-- told me to do it....
  m_Menu_File->items().push_back(Gtk::Menu_Helpers::CheckMenuElem(
    "Show Hidden",Gtk::AccelKey(GDK_H, Gdk::CONTROL_MASK), sigc::mem_fun(*this, &Darimasen::fShowHidden)));

  optShowHidden = (Gtk::CheckMenuItem *)&m_Menu_File->items().back();

  m_Menu_File->items().push_back(Gtk::Menu_Helpers::MenuElem(
    "Print history to Terminal",Gtk::AccelKey(GDK_P, Gdk::CONTROL_MASK), sigc::mem_fun(*this, &Darimasen::fPrintHist)));

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
   
  ViewTree = new Gtk::ToggleToolButton(Gtk::StockID("gtk-index"));
  //ViewTree->signal_clicked().connect(sigc::mem_fun(*this, &Darimasen::fViewTree));
  TopBar.append(*ViewTree);
  ViewTree->set_sensitive(false);
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
 }

/**********************/

Darimasen::~Darimasen(){
  fQuit();
  }

/**********************/

void Darimasen::fQuit(){

  delete Tabber;

  delete BackButton;
  delete ChangeIconMode;
  delete ViewTree;
  delete NewTab;
  DarimasenMenuContainer->remove();

  delete DarimasenMenuContainer;
  delete m_Menu_File;
  delete CompactMenu;
  delete CompactMenuContainer;

  for (int i=0; i < IconModeList.size(); i++){
     delete IconModeList[i];
     delete EventBoxList[i];
     }

  hide();
  }

/**********************/

void Darimasen::fAbout(){
  aboutDa about;
  Gtk::Main::run(about); 
  }

/**********************/

bool Darimasen::aboutDa::ch0wned(GdkEventButton*){
  hide();
  }

/**********************/

Darimasen::aboutDa::aboutDa(){
  image1 = new class Gtk::Image(  Gdk::Pixbuf::create_from_file( 
    (Glib::ustring)DATADIR + (Glib::ustring)"/pixmaps/darimasenInfo.svg" ) );
  closer = new Gtk::EventBox;
  closer->add(*image1);
  closer->signal_button_press_event().connect(
    sigc::mem_fun(*this, &Darimasen::aboutDa::ch0wned));

  add(*closer);
  set_title("About Darimasen");
  set_modal(true);
  set_resizable(false);
  set_decorated (false);
  set_position(Gtk::WIN_POS_CENTER);

  show_all_children();
  }

/**********************/

Darimasen::aboutDa::~aboutDa(){
  delete image1;
  delete closer;
  }



/**********************/

void Darimasen::fShowHidden(){


  DarimasenMenuContainer->remove();
  DaMenu = Gtk::manage( new DarimasenMenu(get_history(Tabber->get_current_page()), *this, Tabber->get_current_page()));
  DarimasenMenuContainer->add(*DaMenu);



for( int i = 0; IconModeList.size() > i; i++)
  IconModeList[i]->SwitchHidden(i);

  }

/**********************/

void Darimasen::fBack(){
  del_history(Tabber->get_current_page());

  ChangeCurrentPath(get_history(Tabber->get_current_page()),false,false);

  if (history[Tabber->get_current_page()].size() == 1)
    BackButton->set_sensitive(false);
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

  void Darimasen::set_message(Glib::ustring in){
   Info.pop();
   Info.push(in);
}

/**********************/


// this is called on file operations - since there is more then one tab,
// all of them should be checked to see if they should be updated.
void Darimasen::updateView(Glib::ustring sourceDir, Glib::ustring targetDir){
//  std::cout << sourceDir << "\n" << targetDir << "\n\n";

  for(int i = 0; i < history.size(); i++){
    if( get_history(i) == sourceDir || get_history(i) == targetDir ){
      ChangeCurrentPath(get_history(i),false,false);
      }
    }
}

/**********************/

void Darimasen::fChangeIconMode(){
  mode = (mode + 1) % 2; // increment, mod of possibilities.
  for(int i = 0; i < history.size(); i++){
      ChangeCurrentPath(get_history(i),false,false);
    } 
  }

/**********************/

Glib::ustring Darimasen::get_history(gint tab, gint level){
  return history[tab][history[tab].size()-level-1];
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

bool Darimasen::del_history(gint tab){
  history[tab].pop_back();
  if ( history[tab].size() == 0 ){
    history.erase(history.begin()+tab,history.begin()+tab+1 );
    return 0;
    }
  return history[tab].size();
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

void Darimasen::fHistoryMenu(gint tab, gint iterations){
  for(int i = 0; i < iterations; i++)
    del_history(Tabber->get_current_page());

  ChangeCurrentPath(get_history(Tabber->get_current_page()),false,false);

  if (history[Tabber->get_current_page()].size() == 1)
    BackButton->set_sensitive(false);
  }

/**********************/
