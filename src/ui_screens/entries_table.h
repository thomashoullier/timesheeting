/** @file
 * @brief EntriesTable definition. */
#ifndef ENTRIES_TABLE_H
#define ENTRIES_TABLE_H

#include "date_selector/date_selector_ncurses.h"
#include "register/register_ncurses.h"
#include "total_bar/total_bar.h"
#include "ui_component.h"

/** @brief Class for holding the table of entries for a given day. */
class EntriesTable : public UIComponent {
public:
  /** @brief Constructor. */
  explicit EntriesTable();

  char input_loop() override;
  void refresh() override;
  void clear() override;
  void update() override;

private:
  /** @brief Handle to the date range selector. */
  DateSelectorNcurses date_selector;
  /** @brief Handle to the total duration display. */
  TotalBar total_bar;
  /** @brief Handle to the register of entries. */
  RegisterNcurses reg;

  /** @brief Rename an entry in the register. */
  void rename_item();
  /** @brief Remove an entry in the register. */
  void remove_item();
};

#endif // ENTRIES_TABLE_H
