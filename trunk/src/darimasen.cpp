/* Darimasen - darimasen.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

/**********************/

#include "darimasen.h"
#include <iostream>

/**********************/

Darimasen::Darimasen(Glib::ustring path)
{
  set_title("Darimasen");
  set_default_size(500, 320);
  iconmode = 0;
  depth = 0;
  filesAtPath = 0;
  DoSomethingWithDaMenu = false;
  Gnome::Vfs::init();
  
    DaMenu.signal_size_allocate().connect (
		sigc::mem_fun (*this, &Darimasen::DaMenu_size_allocate));
    m_Controls.signal_size_allocate().connect (
		sigc::mem_fun (*this, &Darimasen::Menubar_size_allocate));  
  add(m_Box);

  
  Gtk::Menu * m_Menu_File = new Gtk::Menu;
  Gtk::Menu::MenuList& menulist = m_Menu_File->items();

  activeCompact = Gtk::manage(new class Gtk::CheckMenuItem("Active Menu Compaction"));
  activeCompact->set_active(false);
  activeCompact->show();
  activeCompact->signal_toggled().connect(sigc::mem_fun(*this, &Darimasen::fActiveCompaction));
  menulist.push_back( Gtk::Menu_Helpers::CheckMenuElem(*activeCompact) ) ;

  showHidden = false;
  ShowHidden = Gtk::manage(new class Gtk::CheckMenuItem("Show Hidden Files"));
  ShowHidden->set_active(false);
  ShowHidden->show();
  ShowHidden->signal_toggled().connect(sigc::mem_fun(*this, &Darimasen::fShowHidden));
  menulist.push_back( Gtk::Menu_Helpers::CheckMenuElem(*ShowHidden) ) ;
	
  menulist.push_back( Gtk::Menu_Helpers::StockMenuElem(Gtk::Stock::QUIT,
    sigc::mem_fun(*this, &Darimasen::on_menu_file_quit) ) );

  Gtk::Widget * image1 = Gtk::manage(new class Gtk::Arrow(Gtk::ARROW_DOWN ,Gtk::SHADOW_NONE)); 
  CompactMenu.items().push_back( Gtk::Menu_Helpers::ImageMenuElem("", *image1,*m_Menu_File) );
    
  m_Controls.set_show_arrow(false);
  tb0.add(CompactMenu);
  m_Controls.add(tb0); 
  CompactMenu.show();
  tb0.show();
  
  m_Controls.add(separatortoolitem1);
  separatortoolitem1.show();
  
  DaMenuBuilder(resolvePath(path));
  tb1.set_expand(true);


  tb1.add(DaMenu);
  m_Controls.add(tb1);

  
  DaMenu.show();
  tb1.show();
  
  m_Controls.add(separatortoolitem2);
  separatortoolitem2.show();//showHidden
  
  BackButton = Gtk::manage(new class Gtk::ToolButton(Gtk::StockID("gtk-go-back")));
  BackButton->signal_clicked().connect(sigc::mem_fun(*this, &Darimasen::fBackButton));
  BackButton->set_sensitive(false);
  BackButton->set_is_important(true);
  m_Controls.append(*BackButton);
  BackButton->show();
  
  ChangeIconMode = Gtk::manage(new class Gtk::ToolButton(Gtk::StockID("gtk-convert")));
  ChangeIconMode->signal_clicked().connect(sigc::mem_fun(*this, &Darimasen::fChangeIconMode));
  //ChangeIconMode->set_sensitive(false);
  ChangeIconMode->set_is_important(true);
  m_Controls.append(*ChangeIconMode);
  ChangeIconMode->show();
   
  ViewTree = Gtk::manage(new class Gtk::ToggleToolButton(Gtk::StockID("gtk-index")));
  ViewTree->signal_clicked().connect(sigc::mem_fun(*this, &Darimasen::fViewTree));
  //ViewTree -> set_sensitive(false);
  ViewTree -> set_is_important(true);  
  m_Controls.append(*ViewTree);
  ViewTree -> show();

  


  m_Controls.set_toolbar_style(Gtk::TOOLBAR_ICONS); 
  m_Controls.show();
  
  m_Box.pack_start(m_Controls, Gtk::PACK_SHRINK);
  
  m_Box.pack_start(m_hpane);
  m_hpane.show();

  MainScroller.set_flags(Gtk::CAN_FOCUS);
  MainScroller.set_shadow_type(Gtk::SHADOW_NONE);
  
  MainScroller.show();
  
  FileTreeScroller.set_size_request(150,-1);
  FileTreeScroller.set_policy(Gtk::POLICY_ALWAYS, Gtk::POLICY_AUTOMATIC);
  if(false) FileTreeScroller.show(); // come configuration issue








  
  
  MainEventBox = Gtk::manage(new class Gtk::EventBox);

  m_Color.set_rgb(65535, 65535, 65535);
  MainEventBox->modify_bg(Gtk::STATE_NORMAL, m_Color);


  if (iconmode == 0)
    MainScroller.set_policy(Gtk::POLICY_ALWAYS, Gtk::POLICY_AUTOMATIC);



  MainScroller.add(*MainEventBox);
  m_hpane.pack1(FileTreeScroller, Gtk::SHRINK);
  m_hpane.pack2(MainScroller, Gtk::EXPAND|Gtk::SHRINK);
  m_hpane.set_position ( 0 ); 

  m_Statusbar.set_has_resize_grip();
  m_Statusbar.show();
  
  m_Box.pack_start(m_Statusbar,Gtk::PACK_SHRINK);
  m_Box.show();
  

  MainScrollerHeight = 0;
  DaMenuAvailableWidth = 0;

iconBuild();
  }


/**********************/


Darimasen::~Darimasen(){
  }


/**********************/

void Darimasen::on_menu_file_quit(){
  hide(); //Closes the main window
  }


/**********************/


int Darimasen::resolvePath(Glib::ustring givenPath){

#ifdef WIN32
  pathTruncator = getenv("USERPROFILE");

  if(givenPath[1] == ':'){ //standard Win drive.
#else
  pathTruncator = getenv("HOME");

  if(givenPath[0] == '/'){ // root.
#endif     
    fullPath = givenPath;
    }
  else if (givenPath.length() == 0){
    fullPath = pathTruncator;
    }
  else if(givenPath[0] == '~'){ 
    fullPath = pathTruncator + givenPath.substr(1,givenPath.length() - 1);
    }
  else if(givenPath.length() != 0){
  // some cwd action 

    char * huh;
    huh = new char[1000];
    getcwd(huh,1000);
    Glib::ustring hhuh(huh);
    delete huh;
    hhuh += slash;
    if (givenPath == "."){
      fullPath = hhuh;
      }
    else{
      fullPath = hhuh + givenPath;
      }
    }

    // hanging end quote
    while( fullPath.find("\"" ) != Glib::ustring::npos )    
      fullPath.replace(fullPath.find("\""),1,"");

    
    // add the nice trailing slash if needed
    if(fullPath.substr(fullPath.length()-1,1) != slash )
      fullPath += slash;
  
   
    // the double dot-slash
    while( fullPath.find(".."+slash) != Glib::ustring::npos ){
      int x = fullPath.find(".."+slash);
      int y = fullPath.rfind(slash,x-2);
      fullPath.replace(y,x-y+3, slash);
      }       
       
    // the single dot-slash
    while( fullPath.find("." + slash) != Glib::ustring::npos )    
       fullPath.replace(fullPath.find("." + slash),2,"");
    
    // the double-slash
    while( fullPath.find(slash + slash) != Glib::ustring::npos )
       fullPath.replace(fullPath.find(slash + slash),2,slash);


    //test if there is now a legitamate path 
    if( (chdir(fullPath.c_str()) == -1) ){
      fullPath = pathTruncator;
      }
    

  // set up for $HOME shorteneing
  if( (fullPath.find(pathTruncator) == 0) )
    return pathTruncator.length() -1;

  return 0;
  }

/**********************/

void Darimasen::fChangeIconMode(){
  iconmode = (++iconmode)%AvailableModes;

  if (iconmode == 0){
    MainScrollerHeight -= MainScroller.get_hscrollbar()->get_height();
    MainScroller.set_policy(Gtk::POLICY_ALWAYS, Gtk::POLICY_AUTOMATIC);
    }
  else {
    MainScrollerHeight += MainScroller.get_hscrollbar()->get_height();
    MainScroller.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    }
    
  iconBuild();  
  }

/**********************/

void Darimasen::fViewTree(){ 
  if (ViewTree->get_active()){
    m_hpane.hide();

    dirtree * test = new dirtree(fullPath);
    FileTreeScroller.add(*test);
    FileTreeScroller.show();
    m_hpane.set_position ( 150 );
    m_hpane.show();
    }
  else{
    m_hpane.hide();

  Gtk::Widget * tmp = FileTreeScroller.get_child();
  if (tmp)
    delete tmp;

    FileTreeScroller.hide();
    m_hpane.set_position ( 0 );
    m_hpane.show();
    }
  }

/**********************/

int Darimasen::submenuCount(Glib::ustring path){

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
  return j;
  }

/**********************/

void Darimasen::DaMenuSelect(Glib::ustring path){
  DaMenuBuilder(changePath(path,0));
  //DaFileLister(); //5:45pm, 24 Dec 2004, it worked! // leave this comment
  iconBuild();    
  }


/**********************/


void Darimasen::fBackButton(){
  
  Glib::ustring tmpPath = history.top();
  history.pop();
  DaMenuBuilder(changePath(tmpPath,1));
  
  iconBuild();

  if ( history.size() == 0 )
    BackButton->set_sensitive(false);
  }


/**********************/


//  for known-good paths only.
int Darimasen::changePath(Glib::ustring givenPath, bool back){
    
  if ( !back ){
    history.push(fullPath);
    BackButton->set_sensitive(true);
    }
    
  fullPath = givenPath;  
  if(fullPath.substr(fullPath.length()-1,1) != slash ) fullPath += slash;

    
    if( fullPath.find(pathTruncator) == 0)
      return pathTruncator.length() -1;
    return 0;
    }


/**********************/

void Darimasen::fActiveCompaction(){
  if (activeCompact->get_active()){
    m_Controls.set_show_arrow(true);
    }
  else{
    m_Controls.set_show_arrow(false);  
    }
  }

/**********************/

 
void Darimasen::iconBuild(){  
  Gtk::Widget * tmp = MainEventBox->get_child();
  if (tmp)
    delete tmp;
  DaIconModes * Iconmodes = new class DaIconModes(fullPath,filesAtPath,iconmode,showHidden);
  MainEventBox->add(*Iconmodes);
  MainEventBox->get_child()->show();
  MainEventBox->show();
}

/**********************/

void Darimasen::DaMenuBuilder(const int v){
  filesAtPath = 0;
  while( depth-- >= 0) //clean out if needed
  DaMenu.items().pop_back();
  
  depth = 0;
  int x = v;
              
  // determine the number of menus needed 
  while ( (fullPath.find(slash,x) != std::string::npos) ) {
    x = fullPath.find(slash,x) + 1; depth++;
    }
    
  //do the actual ustring breakup    
  Glib::ustring tmp[depth];
  x = v;
  int y = 0;
  int i = 0;

  while ( (fullPath.find(slash,x) !=  std::string::npos)){
    x = fullPath.find(slash,x) + 1;
    tmp[i++] = fullPath.substr(y,x-y);
    y = x;
    }

  Gtk::Menu * MenuRay; //menuArray
  MenuRay = new Gtk::Menu[depth+1]; // +1 for subfolder menu

  Glib::ustring tmpp;
  if(v != 0){ // Shortened? Show it!
    tmpp = "~";
    tmpp += slash;  
    }
  else{
    tmpp = tmp[0];
    }
    
    // The first menu doesn't have a parent, in theory.
    Gtk::Menu::MenuList& menulist = MenuRay[0].items();
    
      menulist.push_back( Gtk::Menu_Helpers::MenuElem("~ (home)",
        sigc::bind<Glib::ustring>(
          sigc::mem_fun(*this, &Darimasen::DaMenuSelect), pathTruncator)));

    //The following DOES actually change with platform!
#ifdef WIN32      
      char driveLetter[] = "A:";
  
      for( unsigned int DriveMask = _getdrives(); DriveMask > 0; DriveMask >>= 1 ){
        if (DriveMask & 1){
          menulist.push_back( Gtk::Menu_Helpers::MenuElem((Glib::ustring)driveLetter + "\\",
            sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &Darimasen::DaMenuSelect), (Glib::ustring)driveLetter )));
          }
        ++driveLetter[0];
        }
#else
      menulist.push_back( Gtk::Menu_Helpers::MenuElem("/ (root)" ,
    sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &Darimasen::DaMenuSelect), "/"  )));
      // Probably some DBUS integration here, get the important
      // mountable drives. 
#endif 
    menulist.push_back( Gtk::Menu_Helpers::SeparatorElem());
    // "bookmarks" code goes here.
    DaMenu.items().push_back(Gtk::Menu_Helpers::MenuElem(tmpp, MenuRay[0]) );
    
    for (i=1; i<depth; i++){
      MenuRay[i].show();
      if (tmp[i].find("_") == Glib::ustring::npos){
        DaMenu.items().push_back(Gtk::Menu_Helpers::MenuElem(tmp[i], MenuRay[i]) );
        }
      else { //correct handling of underscores (_)
        int startAtPos = 0;
        Glib::ustring undrescored = tmp[i];
        while (tmp[i].find("_",startAtPos) != -1){
          startAtPos = tmp[i].find("_", startAtPos);
          undrescored.replace(startAtPos, 1, "__");
          startAtPos ++;   
          }
        DaMenu.items().push_back(Gtk::Menu_Helpers::MenuElem(undrescored, MenuRay[i]) );
        }
      }

    Glib::ustring curdir = fullPath;

    // let's work backwords! (This saves a lot of string-gluing)
    while (i + 1> 1){
    if (i != depth){
      Gtk::MenuItem * subdir = Gtk::manage( new Gtk::MenuItem(tmp[i]));
      subdir->signal_activate().connect( sigc::bind<Glib::ustring>(
        sigc::mem_fun(*this, &Darimasen::DaMenuSelect), curdir + tmp[i] ));
      MenuRay[i].attach(*subdir, 0 ,4, 0, 1);
      subdir->show();
      Gtk::SeparatorMenuItem * sep = Gtk::manage( new Gtk::SeparatorMenuItem());
      MenuRay[i].attach(*sep, 0 ,4, 1, 2);
      sep->show();    
      } 
    

  int pos = 0;// filesAtPath = 0;
  try{
    Gnome::Vfs::DirectoryHandle handle;
    handle.open(curdir, Gnome::Vfs::FILE_INFO_DEFAULT | Gnome::Vfs::FILE_INFO_FOLLOW_LINKS);

    bool file_exists = true;
    while(file_exists) {
      Glib::RefPtr<Gnome::Vfs::FileInfo> refFileInfo = handle.read_next(file_exists);
      if (refFileInfo->get_type() == Gnome::Vfs::FILE_TYPE_DIRECTORY
        && ( (refFileInfo->get_name().substr(0,1) != ".") || showHidden )
        && refFileInfo->get_name() != "."
        && refFileInfo->get_name() != ".."
        ){ 
          Gtk::MenuItem * subdir = Gtk::manage( new Gtk::MenuItem((refFileInfo->get_name() + slash + " ")));
          subdir->signal_activate().connect( sigc::bind<Glib::ustring>(
            sigc::mem_fun(*this, &Darimasen::DaMenuSelect), curdir + refFileInfo->get_name() ));
       
          Glib::ustring SubSubCount = int2ustr(submenuCount(curdir + refFileInfo->get_name()));
  
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
            MenuRay[i].attach(*subsubdir, 3 ,4, pos+2, pos+3);
            subsubdir->show();
            MenuRay[i].attach(*subdir, 0 ,3, pos+2, pos+3);

            subsubdir->set_sensitive(false);
            subdir->show();
            }
          else {
            MenuRay[i].attach(*subdir, 0 ,4, pos+2, pos+3);
            subdir->show();            
            }
          pos++;
          } else if (i == depth) filesAtPath++;
        }
      }
  catch(const Gnome::Vfs::exception& ex){}


      if ( i == depth && pos != 0){
        MenuRay[i].show();
        DaMenu.items().push_back(Gtk::Menu_Helpers::MenuElem(int2ustr(pos) + ">>", MenuRay[i]) );  
        }     
      curdir = curdir.substr(0, curdir.rfind(slash, curdir.length()-2)) + slash;     
      i--;
    }         
  }

/**********************/

 void Darimasen::fShowHidden(){


  if (ShowHidden->get_active()){
    showHidden = true;
    }
  else{
    showHidden = false;
    }

  DaMenuBuilder(changePath(fullPath,1));
  
  iconBuild();
  }
/**********************


bool Darimasen::on_configure_event(GdkEventConfigure* event){
  //std::cout << "The Window happens." << "\n";
  int oldHeight = MainScrollerHeight;
  MainScrollerHeight = event->height
                         - m_Controls.get_height()
                         - m_Statusbar.get_height()
                         - MainScroller.get_hscrollbar()->get_height();

  Gtk::Widget * tmp = MainEventBox->get_child();

  // Are you sure?
  if (!tmp || MainScrollerHeight != oldHeight){
    // Are you really sure? (this saves a lot on the constant-redraw front)
    if ((MainScrollerHeight/58) != (oldHeight/58))
    iconBuild();
    }



    
  return Gtk::Widget::on_configure_event(event);
  }

/**********************/

void Darimasen::Menubar_size_allocate(Gtk::Allocation& allocation){


    DaMenuAvailableWidth = allocation.get_width()
                           - tb0.get_width() - 5 // six extra for control padding
                           - separatortoolitem1.get_width() * 2
                           - BackButton->get_width() * 3;

  if (activeCompact->get_active()){

  if(DoSomethingWithDaMenu){
    if (DaMenuAvailableWidth <= DaMenuRequestWidth)
      std::cout << "too small!" << "\n";
    }
    DoSomethingWithDaMenu = true;

}

}

/**********************/

void Darimasen::DaMenu_size_allocate(Gtk::Allocation& allocation){
DaMenuRequestWidth = allocation.get_width();

  if (activeCompact->get_active()){
  //  std::cout << DaMenuAvailableWidth << " < " << DaMenuRequestWidth << "\n";

DoSomethingWithDaMenu = true;
    if (DaMenuAvailableWidth < DaMenuRequestWidth)
      std::cout << "(DaMenu) too small!" << "\n";

    }
  }

/**********************/

Darimasen::dirtree::dirtree(Glib::ustring path){

  m_refTreeModel = Gtk::TreeStore::create(m_Columns);
  set_model(m_refTreeModel);


  //Fill the TreeView's model
  Gtk::TreeModel::Row row = *(m_refTreeModel->append());
  row[m_Columns.m_col_name] = "Billy Bob";

  Gtk::TreeModel::Row childrow = *(m_refTreeModel->append(row.children()));
  childrow[m_Columns.m_col_name] = "Billy Bob Junior";

  childrow = *(m_refTreeModel->append(row.children()));
  childrow[m_Columns.m_col_name] = "Sue Bob";

  row = *(m_refTreeModel->append());
  row[m_Columns.m_col_name] = "Joey Jojo";


  row = *(m_refTreeModel->append());
  row[m_Columns.m_col_name] = "Rob McRoberts";

  childrow = *(m_refTreeModel->append(row.children()));
  childrow[m_Columns.m_col_name] = "Xavier McRoberts";

  //Add the TreeView's view columns:
  append_column("Directory Tree", m_Columns.m_col_name);
  show();


}

/**********************/
