#ifndef PERIOD_SELECTOR_UI_H
#define PERIOD_SELECTOR_UI_H

#include "period_selector_ncurses.h"
#include "../ui_component.h"

namespace tui {
  class PeriodSelectorUI : public UIComponent {
  public:
    /** @brief Constructor. */
    explicit PeriodSelectorUI ();
    char input_loop() override;
    void refresh() override;
    void clear() override;
    void update() override;

    time_lib::DateRange get_current_date_range();

  private:
    /** @brief Handle to the low-level period selector element. */
    PeriodSelectorNCurses period_selector;

    /** @brief Rename a field. */
    void rename_item ();
  };
} // namespace tui

#endif // PERIOD_SELECTOR_UI_H
