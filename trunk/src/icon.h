/* Darimasen - icon.h - Copyright (C) 2005 Sudrien, GPL */

#ifndef ICON_H
#define ICON_H

#include <glibmm.h>
#include <gtkmm.h>
#include <libgnomevfsmm.h>


/* ~_~; */

class icon : public Gtk::TreeModel::ColumnRecord{
  public:
    //icon(const class directory&, const Glib::ustring&);
    //icon(const class directory&, Glib::RefPtr<const Gnome::Vfs::FileInfo>);
    icon();
    ~icon();
    
    gint run() const;
    gint run_as_text() const;
    gint copy() const;
    gint move() const;
    gint link() const;
    gint unlink() const;
    
    Gtk::EventBox sidecon_format() const;
    Gtk::EventBox list_format() const;
    void detail_format(Gtk::Table&) const;

    gint set_run_action() const;
    gint set_permissions() const;


    Gtk::TreeModelColumn< Glib::RefPtr<Gdk::Pixbuf> > icon_pixmap;
    Gtk::TreeModelColumn< Glib::ustring > file_name; 
    Gtk::TreeModelColumn< Glib::ustring > file_mime;
    Gtk::TreeModelColumn< Glib::ustring > file_size;
    Gtk::TreeModelColumn< guint > file_permissions;
    Gtk::TreeModelColumn< guint > file_owner;
    Gtk::TreeModelColumn< guint > file_group;
    Gtk::TreeModelColumn< bool > file_hidden;


  };

#endif
