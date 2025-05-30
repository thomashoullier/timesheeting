/** @file
 *  @brief ProjectTotalsRegister definition. */
#ifndef PROJECT_TOTALS_REGISTER_H
#define PROJECT_TOTALS_REGISTER_H

#include "core/project_total.h"
#include "ncurses_lib/menu_ncurses.h"
#include "../ui_component.h"
#include <memory>
#include <vector>
// IWYU pragma: no_include <format>

namespace tui {
  /** @brief ProjectTotal register implementation in ncurses. */
  class ProjectTotalsRegister final : public ncurses_lib::MenuNCurses,
                                      public UIComponent {
  public:
    /** @brief Constructor. */
    explicit ProjectTotalsRegister
    (const std::vector<core::ProjectTotal> &totals);
    /** @brief Replace the totals currently displayed. */
    void set_items(const std::vector<core::ProjectTotal> &totals);
    config::NormalActions input_loop () final;
    void refresh() final;
    void clear() final;
    void resize() final;
    void update() final;
    void update_status() final;

  private:
    /** @brief Get the menu items from the provided set of items. */
    std::shared_ptr<std::vector<ncurses_lib::MenuItem>> items_to_menu
    (const std::vector<core::ProjectTotal> &items);
  };
} // namespace tui

#endif // PROJECT_TOTALS_REGISTER_H
