#ifndef CALBOX_H
#define CALBOX_H

#include "gtkmm/button.h"
#include "gtkmm/centerbox.h"
#include "gtkmm/enums.h"

class CalBox : public Gtk::CenterBox {
public:
  CalBox();

private:
  Gtk::Button prev_day;
  Gtk::Button next_day;
  Gtk::Button cal_day;
};

#endif // CALBOX_H
