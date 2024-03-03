#ifndef TABS_H
#define TABS_H

#include "daily_entries/daily_entries.h"
#include "gtkmm/notebook.h"

class Tabs : public Gtk::Notebook {
public:
  Tabs();

private:
  DailyEntries daily_entries;
};

#endif // TABS_H
