#include <iostream>
#include "version.h"

#include <gtkmm/application.h>
#include "examplewindow.h"

int main(int argc, char *argv[]) {
  std::cout << "# timesheeting main #" << std::endl;
  std::cout << "Version: " << TIMESHEETING_VERSION << std::endl;

  auto app = Gtk::Application::create("org.gtkmm.example");
  return app->make_window_and_run<ExampleWindow>(argc, argv);
}
