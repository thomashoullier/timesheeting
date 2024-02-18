#include "application.h"

Application::Application () {
  gui_app = Gtk::Application::create("org.gtkmm." + TIMESHEETING_TITLE);
}

int Application::run_gui () {
  return gui_app->make_window_and_run<TopWindow>(0, nullptr);
}
