#ifndef STATUS_BAR_NCURSES_H
#define STATUS_BAR_NCURSES_H

#include "../status_bar_interface.h"
#include <ncurses.h>

class StatusBarNCurses : public StatusBarInterface {
public:
  StatusBarNCurses ();
  ~StatusBarNCurses ();
  void print (const std::string &msg) override;
  void print_wait(const std::string &msg) override;
  bool query_confirmation (const std::string &msg) override;
private:
  WINDOW *win;

  WINDOW* init_window ();
};

#endif // STATUS_BAR_NCURSES_H
