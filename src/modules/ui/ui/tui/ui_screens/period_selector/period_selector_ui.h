/** @file
 *  @brief PeriodSelectorUI definition. */
#ifndef PERIOD_SELECTOR_UI_H
#define PERIOD_SELECTOR_UI_H

#include "period_selector_ncurses.h"
#include "time_lib/date_range.h"
#include "../ui_component.h"

namespace tui {
  /** @brief UIComponent holding a PeriodSelectorNCurses for getting
             a DateRange from the user. */
  class PeriodSelectorUI final : public UIComponent {
  public:
    /** @brief Constructor. */
    explicit PeriodSelectorUI ();
    config::NormalActions input_loop() final;
    void refresh() final;
    void clear() final;
    void resize() final;
    void update() final;
    void update_status() final;

    /** @brief Read the currently inputted DateRange from the UI. */
    time_lib::DateRange get_current_date_range();

  private:
    /** @brief Handle to the low-level period selector element. */
    PeriodSelectorNCurses period_selector;

    /** @brief Rename a field. */
    void rename_item ();
  };
} // namespace tui

#endif // PERIOD_SELECTOR_UI_H
