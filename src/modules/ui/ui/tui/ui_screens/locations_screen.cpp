#include "locations_screen.h"
#include "config/key.h"
#include "ncurses_lib/menu_ncurses.h"
#include "status_bar/status_bar.h"
#include "db/db_sqlite.h"
#include "log_lib/logger.h"
#include "update_manager.h"
#include "ui/keys/bound_keys.h"

namespace tui {
  LocationsScreen::LocationsScreen()
    : location_col(std::make_unique<Column<core::Location>>
                   (std::vector<core::Location>(),
                    ncurses_lib::WindowPosition::left)),
      show_only_active(true) {
    // TODO: initialize the column directly.
    update_location_col();
  }

  void LocationsScreen::refresh() {
    if (needs_resize)
      resize();
    location_col->refresh();
  }

  void LocationsScreen::clear() { location_col->clear(); }

  void LocationsScreen::resize() {
    log_lib::logger().log("LocationsScreen::resize() called.",
                          log_lib::LogLevel::debug);
    location_col->resize();
    needs_resize = false;
  }

  void LocationsScreen::update() {
    log_lib::logger().log("LocationsScreen update.", log_lib::LogLevel::debug);
    update_location_col();
  }

  config::NormalActions LocationsScreen::input_loop() {
    location_col->set_border();
    status().print(location_col->get_current_item_string());
    while (true) {
      auto ch = location_col->get_input();
      auto kb = keys::BoundKeys::get().kb;
      auto action = keys::BoundKeys::get().kb.normal_mode.action_requested(ch);
      switch (action) {
      case config::NormalActions::down:
        if (location_col->select_down_item() ==
            ncurses_lib::MenuNCurses::ItemSelectionStatus::changed) {
          status().print(location_col->get_current_item_string());
        }
        break;
      case config::NormalActions::up:
        if (location_col->select_up_item() ==
            ncurses_lib::MenuNCurses::ItemSelectionStatus::changed) {
          status().print(location_col->get_current_item_string());
        }
        break;
      case config::NormalActions::add:
        if (not(add_item())) {
          status().print_wait("DB logic error! Nothing was done to the DB.");
        } else {
          UpdateManager::get().locations_have_changed();
          status().print(location_col->get_current_item_string());
        }
        break;
      case config::NormalActions::rename:
        if (not(rename_item())) {
          status().print_wait("DB logic error! Nothing was done to the DB.");
        } else {
          UpdateManager::get().locations_have_changed();
          status().print(location_col->get_current_item_string());
        }
        break;
      case config::NormalActions::remove:
        remove_item();
        status().print(location_col->get_current_item_string());
        break;
      case config::NormalActions::active_toggle:
        toggle_active_item();
        status().print(location_col->get_current_item_string());
        break;
      case config::NormalActions::active_visibility:
        toggle_archive_visibility();
        status().print(location_col->get_current_item_string());
        break;
      case config::NormalActions::projects_screen:
      case config::NormalActions::entries_screen:
      case config::NormalActions::project_report_screen:
      case config::NormalActions::weekly_report_screen:
      case config::NormalActions::duration_display:
      case config::NormalActions::quit:
        location_col->unset_border();
        return action;
      default: // Do nothing
        break;
      }
    }
  }

  void LocationsScreen::update_location_col() {
    std::vector<core::Location> location_items;
    if (show_only_active)
      location_items = db::db().query_locations_active();
    else
      location_items = db::db().query_locations();
    location_col->set_items(location_items);
    location_col->refresh();
  }

  bool LocationsScreen::add_item() {
    auto new_item_name = status().get_user_string();
    if (new_item_name.empty())
      return true;
    auto success = db::db().add_location(new_item_name);
    log_lib::logger().log("Added location: " + new_item_name,
                          log_lib::LogLevel::info);
    update_location_col();
    return success;
  }

  bool LocationsScreen::rename_item() {
    try {
      auto id = location_col->get_current_id();
      auto new_item_name = status().get_user_string();
      if (new_item_name.empty())
        return true;
      auto success = db::db().edit_location_name(id, new_item_name);
      update_location_col();
      return success;
    } catch (ncurses_lib::MenuEmpty &e) {
      // TODO: instead of managing this by exception, try to check if the column
      // is empty explicitely.
      return true;
    }
  }

  void LocationsScreen::remove_item() {
    try {
      auto id = location_col->get_current_id();
      bool user_conf = status().query_confirmation("Remove item? (Y/N)");
      if (!user_conf)
        return;
      db::db().delete_location(id);
      update_location_col();
    } catch (ncurses_lib::MenuEmpty &e) {
      return;
    }
  }

  void LocationsScreen::toggle_active_item() {
    try {
      auto id = location_col->get_current_id();
      db::db().toggle_location_active(id);
      update_location_col();
    } catch (ncurses_lib::MenuEmpty &e) {
      return;
    }
  }

  void LocationsScreen::toggle_archive_visibility() {
    show_only_active = !show_only_active;
    update_location_col();
  }
} // namespace tui
