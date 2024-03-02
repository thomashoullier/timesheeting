#include "tabs.h"
#include "gtkmm/enums.h" // TODO
#include "gtkmm/box.h"   // TODO
#include "gtkmm/centerbox.h"                         //
#include "gtkmm/calendar.h" // TODO
#include "gtkmm/object.h"
#include "gtkmm/button.h"
#include "glibmm/date.h"

Tabs::Tabs() {
  auto label_daily = Gtk::make_managed<Gtk::Label>("Daily entries");
  auto label_hierarchy = Gtk::make_managed<Gtk::Label>("Hierarchy items");
  auto label_projects = Gtk::make_managed<Gtk::Label>("Project totals");
  auto label_weekly = Gtk::make_managed<Gtk::Label>("Weekly report");
  auto label_export = Gtk::make_managed<Gtk::Label>("Export");

  // Implementing the Daily entries screen.
  auto daily_vbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
  auto cal_hbox = Gtk::make_managed<Gtk::CenterBox>();

  // Prev/next arrows
  auto prev_day = Gtk::make_managed<Gtk::Button>();
  prev_day->set_icon_name("go-previous");
  auto next_day = Gtk::make_managed<Gtk::Button>();
  next_day->set_icon_name("go-next");
  // Day display + cal dialog opening
  auto cal_day = Gtk::make_managed<Gtk::Button>();
  auto day_str =
    Glib::Date(Glib::Date::Day(2),Glib::Date::Month(3),Glib::Date::Year(2024));
  cal_day->set_label(day_str.format_string("%a, %d %b %Y"));

  cal_hbox->set_start_widget(*prev_day);
  cal_hbox->set_center_widget(*cal_day);
  cal_hbox->set_end_widget(*next_day);

  auto cal = Gtk::make_managed<Gtk::Calendar>();
  daily_vbox->append(*cal_hbox);

  this->append_page(*daily_vbox, "Daily entries");


  this->append_page(*label_hierarchy, "Hierarchy items");
  this->append_page(*label_projects, "Project total");
  this->append_page(*label_weekly, "Weekly report");
  this->append_page(*label_export, "Export");
  this->set_expand(true);
}
