#include "entries_screen.h"
#include "config/key.h"
#include "entries_table.h"
#include "stopwatch/stopwatch_ui.h"
#include "log_lib/logger.h"
#include <iostream>

namespace tui {
  EntriesScreen::EntriesScreen()
    : stopwatch_ui(std::make_unique<StopwatchUI>()),
      entries_table(std::make_unique<EntriesTable>()) {}

  config::NormalActions EntriesScreen::input_loop() {
    UIComponent *cur_focus{entries_table.get()};
    while (true) {
      auto action = cur_focus->input_loop();
      switch (action) {
      case config::NormalActions::subtabs:
        cur_focus = (cur_focus == stopwatch_ui.get()) ? entries_table.get()
                                                      : stopwatch_ui.get();
        break;
      case config::NormalActions::commit_entry:
        entries_table->update(); // Update request is passed
        break;
      default:
        return action;
      }
    }
  }

  void EntriesScreen::refresh() {
    if (needs_update)
      update();
    stopwatch_ui->refresh();
    entries_table->refresh();
  };

  void EntriesScreen::clear() {
    stopwatch_ui->clear();
    entries_table->clear();
  };

  void EntriesScreen::update() {
    log_lib::logger().log("EntriesScreen update.",
                          log_lib::LogLevel::debug);
    stopwatch_ui->update();
    entries_table->update();
    needs_update = false;
  };
} // namespace tui
