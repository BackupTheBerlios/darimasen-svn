/* Darimasen - darimasen.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

/**********************/

#include "darimasen.h"
#include <iostream>

/**********************/

Darimasen::Darimasen(std::vector<Glib::ustring> path){
  set_title("Darimasen");
  set_default_size(500, 330);
  numOfTabs = 0;

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

  Gtk::Widget * image1 = Gtk::manage(new class Gtk::Arrow(Gtk::ARROW_DOWN ,Gtk::SHADOW_NONE)); 
  CompactMenu->items().push_back( Gtk::Menu_Helpers::ImageMenuElem("", *image1,*m_Menu_File) );
    
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
  DaMenu = Gtk::manage(new DarimasenMenu(path[0]));
  DarimasenMenuContainer->add(*DaMenu);

  Gtk::SeparatorToolItem * sep2  = Gtk::manage(new Gtk::SeparatorToolItem);
  TopBar.append(*sep2);
  sep2->show();

  Gtk::ToolButton * BackButton = Gtk::manage(new Gtk::ToolButton(Gtk::StockID("gtk-go-back")));
  //BackButton->signal_clicked().connect(sigc::mem_fun(*this, &Darimasen::fBackButton));
  TopBar.append(*BackButton);
  BackButton->show();
  
  Gtk::ToolButton * ChangeIconMode = Gtk::manage(new Gtk::ToolButton(Gtk::StockID("gtk-convert")));
  //ChangeIconMode->signal_clicked().connect(sigc::mem_fun(*this, &Darimasen::fChangeIconMode));
  TopBar.append(*ChangeIconMode);
  ChangeIconMode->show();
   
  Gtk::ToggleToolButton * ViewTree = Gtk::manage(new Gtk::ToggleToolButton(Gtk::StockID("gtk-index")));
  //ViewTree->signal_clicked().connect(sigc::mem_fun(*this, &Darimasen::fViewTree));
  TopBar.append(*ViewTree);
  ViewTree -> show();

  VerticalOrganizer.add(HideTreePane);
  HideTreePane.show();

  HideTreePane.pack1(TreeScroller, Gtk::SHRINK);

  Tabber = Gtk::manage(new Gtk::Notebook);
  Tabber->show();

  HideTreePane.pack2(*Tabber, Gtk::EXPAND|Gtk::SHRINK);
   
  Info.set_has_resize_grip();
  Info.show();
  VerticalOrganizer.pack_start(Info, Gtk::PACK_SHRINK);

  show();

  //std::cout << path << "\n";
  for(int i = 0; i < path.size(); i++)
    addTab(path[i]);
  }

/**********************/

Darimasen::~Darimasen(){
  
  hide();
  }

/**********************/

void Darimasen::addTab(Glib::ustring path){
  if( numOfTabs == 0 )
    Tabber->set_show_tabs(false);
  else
    Tabber->set_show_tabs(true);
  numOfTabs++;

  Gtk::Label * tabNum = Gtk::manage(new Gtk::Label(path.substr(path.rfind(slash, path.length() -2)+1)));

  Gtk::EventBox * MainEventBox = Gtk::manage(new class Gtk::EventBox);

  Gdk::Color m_Color;
  m_Color.set_rgb(65535, 65535, 65535);
  MainEventBox->modify_bg(Gtk::STATE_NORMAL, m_Color);
  MainEventBox->show();

  DaIconModes * foo;
  foo = new DaIconModes(path);
  Gtk::ScrolledWindow * MainScroller = Gtk::manage(new Gtk::ScrolledWindow);
  MainScroller->show();
  MainScroller->add(*MainEventBox);
  MainEventBox->add(*foo);
  MainScroller->set_policy(Gtk::POLICY_ALWAYS, Gtk::POLICY_AUTOMATIC);
  foo->show();
  Tabber->append_page( *MainScroller, *tabNum );
  }

/**********************/

Darimasen::DarimasenMenu::DarimasenMenu(const Glib::ustring path){
  depth = 0;
  Glib::ustring shortpath = path; //home = getenv("HOME");
  
  int startPos = 0 , i = 0;


  if ( path.find(getenv("HOME")) != Glib::ustring::npos ){
  shortpath.replace(0,((Glib::ustring)getenv("HOME")).length(),"~");

  }


  while ( (shortpath.find(slash,startPos) != Glib::ustring::npos) ) {
    startPos = shortpath.find(slash,startPos) + 1; depth++;
    }

  Glib::ustring menulevel[depth];

  startPos = 0;
  for(i = 0; shortpath.find(slash,startPos) !=  Glib::ustring::npos ; i++ ){
    menulevel[i] = shortpath.substr(startPos,shortpath.find(slash,startPos) - startPos + 1 );
    startPos = shortpath.find(slash,startPos) + 1;

    };

  Gtk::Menu * MenuRay = Gtk::manage(new Gtk::Menu[depth+1]); // +1 for subfolder menu
  items().push_front(Gtk::Menu_Helpers::MenuElem( ">>", MenuRay[depth]));
  for(i = depth - 1; i>=0; i--){
    std::cout << menulevel[i] << "\n";
    items().push_front(Gtk::Menu_Helpers::MenuElem( underscoreSafe(menulevel[i]), MenuRay[i]));
    };

  show();
}

/**********************/

Gtk::Menu Darimasen::DarimasenMenu::MenuForPath(Glib::ustring){}

/**********************/

Glib::ustring Darimasen::DarimasenMenu::CountSubdir(Glib::ustring){}

/**********************/




/**********************/
/**********************/
/**********************/
/**********************/
/**********************/
/**********************/
/**********************/
/**********************/
