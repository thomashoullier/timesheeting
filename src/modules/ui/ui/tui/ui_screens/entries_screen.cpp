#include "entries_screen.h"
#include "config/key.h"
#include "entries_table.h"
#include "stopwatch/stopwatch_ui.h"
#include "log_lib/logger.h"
#include "ui/tui/ui_screens/ui_component.h"
#include <memory>
#include <string>

namespace tui {
EntriesScreen::EntriesScreen()
    : entries_table(std::make_unique<EntriesTable>()),
      stopwatch_ui(std::make_unique<StopwatchUI>
                   (entries_table->day_selector)),
      cur_focus{entries_table.get()} {}

config::NormalActions EntriesScreen::input_loop() {
  while (true) {
    auto action = cur_focus->input_loop();
    switch (action) {
    case config::NormalActions::subtabs:
      cur_focus = (cur_focus == stopwatch_ui.get()) ? entries_table.get()
                                                    : stopwatch_ui.get();
      break;
    case config::NormalActions::next:
      entries_table->select_next_day();
      break;
    case config::NormalActions::previous:
      entries_table->select_previous_day();
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
    if (needs_resize)
      resize();
    if (needs_update)
      update();
    stopwatch_ui->refresh();
    entries_table->refresh();
  }

  void EntriesScreen::clear() {
    stopwatch_ui->clear();
    entries_table->clear();
  }

  void EntriesScreen::resize() {
    log_lib::logger().log("EntriesScreen::resize() called.",
                          log_lib::LogLevel::debug);
    stopwatch_ui->resize();
    entries_table->resize();
    needs_resize = false;
  }

  void EntriesScreen::update() {
    log_lib::logger().log("EntriesScreen update.",
                          log_lib::LogLevel::debug);
    stopwatch_ui->update();
    entries_table->update();
    needs_update = false;
  }

  void EntriesScreen::update_status() {
    if (cur_focus == entries_table.get())
      entries_table->update_status();
    else
      stopwatch_ui->update_status();
  }
} // namespace tui
