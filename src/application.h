#ifndef APPLICATION_H
#define APPLICATION_H

#include <gtkmm/application.h>
#include "glibmm/refptr.h"
#include "globals.h"
#include "topwindow.h"

class Application {
public:
  Application();
  int run_gui();

private:
  Glib::RefPtr<Gtk::Application> gui_app;
};

#endif // APPLICATION_H
