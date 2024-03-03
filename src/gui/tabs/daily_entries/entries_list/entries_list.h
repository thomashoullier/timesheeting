#ifndef ENTRIES_LIST_H
#define ENTRIES_LIST_H

#include "glibmm/object.h"
#include "glibmm/refptr.h"
#include "gtkmm/columnview.h"
#include "gtkmm/singleselection.h"
#include "gtkmm/listitem.h"

// Object to put in the rows
class MyObject : public Glib::Object {
public:
  enum NamePart { FIRST, SECOND };

  static Glib::RefPtr<MyObject> create(const Glib::ustring &first_name,
                                       const Glib::ustring &second_name) {
    return Glib::make_refptr_for_instance<MyObject>(
        new MyObject{first_name, second_name});
  }

  void set_name(const Glib::ustring &first_name,
                const Glib::ustring &second_name) {
    m_name[FIRST] = first_name;
    m_name[SECOND] = second_name;
  }

  Glib::ustring get_name(NamePart name_part) const { return m_name[name_part]; }

protected:
  MyObject(const Glib::ustring &first_name, const Glib::ustring &second_name)
      : m_name{first_name, second_name} {}

private:
  Glib::ustring m_name[2];
};

class EntriesList : public Gtk::ColumnView {
public:
  EntriesList();

protected:
  void on_setup_listitem(const Glib::RefPtr<Gtk::ListItem> &list_item);
  void on_bind_listitem(const Glib::RefPtr<Gtk::ListItem>& list_item,
                        MyObject::NamePart name_part);
};

#endif // ENTRIES_LIST_H
