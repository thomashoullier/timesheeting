#include "entries_list.h"
#include "giomm/liststore.h"
#include "glibmm/refptr.h"
#include "glibmm/ustring.h"
#include "gtkmm/columnviewcolumn.h"
#include "gtkmm/listitem.h"
#include "gtkmm/signallistitemfactory.h"
#include "gtkmm/label.h"
#include <memory>

EntriesList::EntriesList() {
  auto store = Gio::ListStore<MyObject>::create();
  store->append(MyObject::create("Joe", "Brown"));
  store->append(MyObject::create("Mary", "West"));
  store->append(MyObject::create("Danny", "Jones"));

  this->set_model(Gtk::SingleSelection::create(store));
  this->add_css_class("data-table");

  // First column
  auto factory = Gtk::SignalListItemFactory::create();
  factory->signal_setup().connect(
      sigc::mem_fun(*this, &EntriesList::on_setup_listitem));
  factory->signal_bind().connect(sigc::bind(
      sigc::mem_fun(*this, &EntriesList::on_bind_listitem),
      MyObject::FIRST));
  auto col = Gtk::ColumnViewColumn::create("First Name", factory);
  this->append_column(col);

  // Second column
  factory = Gtk::SignalListItemFactory::create();
  factory->signal_setup().connect(
      sigc::mem_fun(*this, &EntriesList::on_setup_listitem));
  factory->signal_bind().connect(sigc::bind(
      sigc::mem_fun(*this, &EntriesList::on_bind_listitem),
      MyObject::SECOND));
  col = Gtk::ColumnViewColumn::create("Second Name", factory);
  this->append_column(col);
}

void EntriesList::on_setup_listitem(const Glib::RefPtr<Gtk::ListItem>
                                    &list_item) {
  list_item->set_child(*Gtk::make_managed<Gtk::Label>());
}

void EntriesList::on_bind_listitem(const Glib::RefPtr<Gtk::ListItem> &list_item,
                                   MyObject::NamePart name_part) {
  auto label = dynamic_cast<Gtk::Label*>(list_item->get_child());
  if (label) {
    auto item = list_item->get_item();
    if (auto mo = std::dynamic_pointer_cast<MyObject>(item)) {
      label->set_text(mo->get_name(name_part));
    }
  }
}
