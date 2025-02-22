/** @file
 * @brief EntriesScreen definition. */
#ifndef ENTRIES_SCREEN_H
#define ENTRIES_SCREEN_H

#include "config/key.h"
#include "ui_screen.h"
#include <memory>

namespace tui {
  /** @brief Entry UI screen. */
  class EntriesScreen : public UIScreen {
  public:
    /** @brief Constructor. */
    explicit EntriesScreen();

    config::NormalActions input_loop() override;
    void refresh() override;
    void clear() override;
    void resize() override;
    void update() override;

  private:
    /** @brief Handle to the stopwatch. */
    std::unique_ptr<UIComponent> stopwatch_ui;
    /** @brief Handle to the table holding the entries. */
    std::unique_ptr<UIComponent> entries_table;
  };
} // namespace tui

#endif // ENTRIES_SCREEN_H
