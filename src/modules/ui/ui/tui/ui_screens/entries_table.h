/** @file
 * @brief EntriesTable definition. */
#ifndef ENTRIES_TABLE_H
#define ENTRIES_TABLE_H

#include "date_selector/day_selector.h"
#include "register/register_ncurses.h"
#include "total_bar/total_bar.h"
#include "ui_component.h"
#include <memory>

namespace tui {
  /** @brief Class for holding the table of entries for a given day. */
  class EntriesTable final : public UIComponent {
  public:
    /** @brief Handle to the day selector. */
    std::shared_ptr<DaySelector> day_selector;

    /** @brief Constructor. */
    explicit EntriesTable();

    config::NormalActions input_loop() final;
    void refresh() final;
    void clear() final;
    void resize() final;
    void update() final;
    void update_status() final;
    /** @brief Switch the display to the next day. */
    void select_next_day();
    /** @brief Switch the display to the previous day. */
    void select_previous_day();

  private:
    /** @brief Handle to the total duration display. */
    TotalBar total_bar;
    /** @brief Handle to the register of entries. */
    RegisterNcurses reg;

    /** @brief Rename an entry in the register. */
    bool rename_item();
    /** @brief Remove an entry in the register. */
    bool remove_item();
  };
} // namespace tui

#endif // ENTRIES_TABLE_H
