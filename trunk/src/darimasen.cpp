/* Darimasen - darimasen.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

/**********************/

#include "darimasen.h"
#include <iostream>

/**********************/

Darimasen::Darimasen(std::vector<Glib::ustring> paths){
  set_title("Darimasen");
  set_default_size(500, 330);
  numOfTabs = 0;
  path = paths;

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
  //DaMenu = Gtk::manage(new DarimasenMenu(path[0]));
  //DarimasenMenuContainer->add(*DaMenu);

  Gtk::SeparatorToolItem * sep2  = Gtk::manage(new Gtk::SeparatorToolItem);
  TopBar.append(*sep2);
  sep2->show();

  Gtk::ToolButton * BackButton = Gtk::manage(new Gtk::ToolButton(Gtk::StockID("gtk-go-back")));
  //BackButton->signal_clicked().connect(sigc::mem_fun(*this, &Darimasen::fBackButton));
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

  //std::cout << path << "\n";
  for(int i = 0; i < path.size(); i++)
    addTab(path[i]);
  }

/**********************/

Darimasen::~Darimasen(){
  
  hide();
  }

/**********************/

void Darimasen::fNewTab(){
#ifdef WIN32
  path.push_back(getenv("USERPROFILE") + slash);
  addTab(getenv("USERPROFILE") + slash);
#else
  path.push_back(getenv("HOME") + slash);
  addTab(getenv("HOME") + slash);
#endif
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
  MainScroller->set_shadow_type(Gtk::SHADOW_NONE);
  MainScroller->add(*MainEventBox);
  MainEventBox->add(*foo);
  MainScroller->set_policy(Gtk::POLICY_ALWAYS, Gtk::POLICY_AUTOMATIC);
  foo->show();
  Tabber->append_page( *MainScroller, *tabNum );
  }

/**********************/

Darimasen::DarimasenMenu::DarimasenMenu(const Glib::ustring path){
  depth = 0;
  showHidden = false;
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
  items().push_front(Gtk::Menu_Helpers::MenuElem( CountSubdir(path) + ">>", MenuRay[depth]));

  Glib::ustring crop = path;


  for(i = depth - 1; i>=0; i--){
    //std::cout << menulevel[i] << "\n";
    items().push_front(Gtk::Menu_Helpers::MenuElem( underscoreSafe(menulevel[i]), MenuRay[i]));
    if(crop == path)
      MenuForPath(MenuRay[i+1], crop);
    else
      MenuForPath(MenuRay[i+1], crop, menulevel[i+1]);
    std::cout << "!" << crop << "\n";
    crop = crop.substr(0, crop.rfind(slash,crop.length()-2)) + slash;

    };

  show();
}

/**********************/

int Darimasen::DarimasenMenu::MenuForPath(
       Gtk::Menu& tmp,
       Glib::ustring& path,
       Glib::ustring current){

  int entry = 0;

  if( current != ""){
    Gtk::MenuItem * subdir = Gtk::manage( new Gtk::MenuItem(path + current + " "));
    tmp.attach(*subdir, 0 ,4, entry++, entry+1);
    subdir->show();
    Gtk::SeparatorMenuItem * sep = Gtk::manage( new Gtk::SeparatorMenuItem());
    tmp.attach(*sep, 0 ,4, entry++, entry+1);
    sep->show();
    }

  try{
    Gnome::Vfs::DirectoryHandle handle;
    handle.open(path, Gnome::Vfs::FILE_INFO_DEFAULT | Gnome::Vfs::FILE_INFO_FOLLOW_LINKS);

    bool file_exists = true;
    while(file_exists) {
      Glib::RefPtr<Gnome::Vfs::FileInfo> refFileInfo = handle.read_next(file_exists);
      if (refFileInfo->get_type() == Gnome::Vfs::FILE_TYPE_DIRECTORY
        && ( (refFileInfo->get_name().substr(0,1) != ".") || showHidden )
        && refFileInfo->get_name() != "."
        && refFileInfo->get_name() != ".."
        ){ 
          Gtk::MenuItem * subdir = Gtk::manage( new Gtk::MenuItem((refFileInfo->get_name() + slash + " ")));
        //  subdir->signal_activate().connect( sigc::bind<Glib::ustring>(
        //    sigc::mem_fun(*this, &Darimasen::DaMenuSelect), curdir + refFileInfo->get_name() ));
       
          Glib::ustring SubSubCount = CountSubdir(path + refFileInfo->get_name());
  
          if (SubSubCount != "0"){
            Gtk::Label * SubSubLabel = Gtk::manage(new class Gtk::Label(SubSubCount));
            SubSubLabel->show();
            Gtk::Arrow * SubSubArrow = Gtk::manage(new class Gtk::Arrow(Gtk::ARROW_RIGHT, Gtk::SHADOW_OUT));     
            SubSubArrow->set_alignment(1,0.5);
            SubSubArrow->set_padding(0,0);
            SubSubArrow->show();
            Gtk::HBox * SubSubHbox = Gtk::manage(new class Gtk::HBox());     
            SubSubHbox->show();
              
            // the following has appearance considerations - esp.
            // pack_end and menu more than 2 wide.
            SubSubHbox->pack_end(*SubSubArrow, Gtk::PACK_SHRINK, 0);
            SubSubHbox->pack_end(*SubSubLabel, Gtk::PACK_SHRINK, 0);
            Gtk::MenuItem * subsubdir = Gtk::manage( new Gtk::MenuItem(*SubSubHbox));
            subsubdir->set_right_justified();

           // subsubdir->signal_activate().connect( sigc::bind<Glib::ustring>(
           // sigc::mem_fun(*this, &Darimasen::DaMenuSelect), path + refFileInfo->get_name() ));


            
            tmp.attach(*subsubdir, 3 ,4, entry, entry+1);
            subsubdir->show();
            tmp.attach(*subdir, 0 ,3, entry++, entry+1);

            subsubdir->set_sensitive(false);
            subdir->show();
            }
          else {
            tmp.attach(*subdir, 0 ,4, entry++, entry+1);
            subdir->show();            
            }
          }
        }
      }
  catch(const Gnome::Vfs::exception& ex){}

    return 0;
  }

/**********************/

Glib::ustring Darimasen::DarimasenMenu::CountSubdir(const Glib::ustring& path){
  int j=0;
  try{
    Gnome::Vfs::DirectoryHandle handle;
    handle.open(path, Gnome::Vfs::FILE_INFO_DEFAULT | Gnome::Vfs::FILE_INFO_FOLLOW_LINKS);

    bool file_exists = true;
    while(file_exists){

      Glib::RefPtr<Gnome::Vfs::FileInfo> refFileInfo = handle.read_next(file_exists);
      if (refFileInfo->get_type() == Gnome::Vfs::FILE_TYPE_DIRECTORY
        && ( (refFileInfo->get_name().substr(0,1) != ".") || showHidden )
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

 
void Darimasen::tabberSwitched(GtkNotebookPage* sig, guint n){
  std::cout << "#" << n << " sig\n";

  CurrentPath = n;
  Gtk::Widget * tmp = DarimasenMenuContainer->get_child();

  if (!tmp){
    DaMenu = new DarimasenMenu(path[n]);
    DarimasenMenuContainer->add(*DaMenu);
    }
  else{
DarimasenMenuContainer->remove();
    DaMenu = new DarimasenMenu(path[n]);
    DarimasenMenuContainer->add(*DaMenu);
    }
  }


/**********************/
/**********************/
/**********************/
/**********************/
/**********************/
/**********************/
/**********************/
/**********************/
