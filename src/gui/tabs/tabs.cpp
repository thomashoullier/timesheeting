#include "tabs.h"

Tabs::Tabs () {
  auto label_daily = Gtk::make_managed<Gtk::Label>("Daily entries");
  auto label_hierarchy = Gtk::make_managed<Gtk::Label>("Hierarchy items");
  auto label_projects = Gtk::make_managed<Gtk::Label>("Project totals");
  auto label_weekly = Gtk::make_managed<Gtk::Label>("Weekly report");
  auto label_export = Gtk::make_managed<Gtk::Label>("Export");

  this->append_page(*label_daily, "Daily entries");
  this->append_page(*label_hierarchy, "Hierarchy items");
  this->append_page(*label_projects, "Project total");
  this->append_page(*label_weekly, "Weekly report");
  this->append_page(*label_export, "Export");
  this->set_expand(true);
}
