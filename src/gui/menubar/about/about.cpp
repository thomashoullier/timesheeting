#include "about.h"

About::About() {
  this->set_program_name(TIMESHEETING_TITLE);
  this->set_decorated(false);
  this->set_version("Version: " + TIMESHEETING_VERSION);
  this->set_website(TIMESHEETING_WEBSITE);
  this->set_website_label("Project link");
  this->set_modal(true);
  this->set_hide_on_close(true);
}
