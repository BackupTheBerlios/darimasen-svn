/* Darimasen - tabs.h - Copyright (C) 2005 Sudrien, GPL */

#ifndef TABS_H
#define TABS_H

#include <gtkmm.h>
#include <vector>
#include "supplimental.h"
#include "directory.h"

class tabs : public Gtk::Notebook {
  public:
    tabs(const std::vector<Glib::ustring>&);
    ~tabs();


    void set_history(gint, Glib::ustring);
    Glib::ustring get_history(gint, gint = 0) const;
    gint rm_history(gint);

    Glib::RefPtr<Gdk::Pixbuf> get_icon(Glib::ustring);

  private:
    void add_tab(gint);
    //signal_switch_page().connect(sigc::mem_fun(*this, &Darimasen::tabberSwitched));

    std::vector< std::vector <Glib::ustring> > history;

    std::vector< Glib::ustring > mimeList;
    std::vector< Glib::RefPtr<Gdk::Pixbuf> > unsizedImg;
};

#endif
