/** @file
 * @brief EntriesScreen definition. */
#ifndef ENTRIES_SCREEN_H
#define ENTRIES_SCREEN_H

#include "ui/tui/ui_screens/entries_table.h"
#include "ui_screen.h"
#include "ui_component.h"
#include <memory>

namespace tui {
  /** @brief Entry UI screen. */
  class EntriesScreen final : public UIScreen {
  public:
    /** @brief Constructor. */
    explicit EntriesScreen();

    config::NormalActions input_loop() final;
    void refresh() final;
    void clear() final;
    void resize() final;
    void update() final;
    void update_status() final;

  private:
    /** @brief Handle to the table holding the entries. */
    std::unique_ptr<EntriesTable> entries_table;
    /** @brief Handle to the stopwatch. */
    std::unique_ptr<UIComponent> stopwatch_ui;
    /** @brief Current focus (between stopwatch and entries register). */
    UIComponent *cur_focus;
  };
} // namespace tui

#endif // ENTRIES_SCREEN_H
