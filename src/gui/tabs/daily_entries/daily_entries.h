#ifndef DAILY_ENTRIES_H
#define DAILY_ENTRIES_H

#include "gtkmm/box.h"
#include "gtkmm/enums.h"
#include "calbox/calbox.h"
#include "entries_list/entries_list.h"

class DailyEntries : public Gtk::Box {
public:
  DailyEntries();

private:
  CalBox cal_box;
  EntriesList entries_list;
  // Stopwatch stopwatch;
};

#endif // DAILY_ENTRIES_H
