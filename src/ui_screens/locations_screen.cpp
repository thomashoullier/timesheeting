#include "locations_screen.h"
#include "status_bar/status_bar_ncurses.h"
#include "../db/db_sqlite.h"
#include "../logger/logger.h"
#include "update_manager.h"

LocationsScreen::LocationsScreen()
    : location_col(std::make_unique<Column<Location>>(std::vector<Location>(),
                                                      WindowPosition::left)),
      show_only_active(true) {
  // TODO: initialize the column directly.
  update_location_col();
}

void LocationsScreen::refresh() { location_col->refresh(); };
void LocationsScreen::clear() { location_col->clear(); };
void LocationsScreen::update() {
  logger().log("LocationsScreen update.", LogLevel::debug);
  update_location_col();
};

char LocationsScreen::input_loop() {
  location_col->set_border();
  while (true) {
    status().print(location_col->get_current_item_string());
    auto ch = location_col->get_input();
    switch (ch) {
    case 'n':
      location_col->select_down_item();
      break;
    case 'e':
      location_col->select_up_item();
      break;
    case 'a':
      if (not(add_item())) {
        status().print_wait("DB logic error! Nothing was done to the DB.");
      } else {
        UpdateManager::get().locations_have_changed();
      }
      break;
    case 'r':
      if (not(rename_item())) {
        status().print_wait("DB logic error! Nothing was done to the DB.");
      } else {
        UpdateManager::get().locations_have_changed();
      }
      break;
    case 'x':
      remove_item();
      break;
    case 'b':
      toggle_active_item();
      break;
    case '.':
      toggle_archive_visibility();
      break;
    default:
      location_col->unset_border();
      return ch;
    }
  }
}

void LocationsScreen::update_location_col() {
  std::vector<Location> location_items;
  if (show_only_active)
    location_items = db().query_locations_active();
  else
    location_items = db().query_locations();
  location_col->set_items(location_items);
  location_col->refresh();
}

bool LocationsScreen::add_item() {
  auto new_item_name = status().get_user_string();
  if (new_item_name.empty())
    return true;
  auto success = db().add_location(new_item_name);
  logger().log("Added location: " + new_item_name, LogLevel::info);
  update_location_col();
  return success;
}

bool LocationsScreen::rename_item() {
  try {
    auto id = location_col->get_current_id();
    auto new_item_name = status().get_user_string();
    if (new_item_name.empty())
      return true;
    auto success = db().edit_location_name(id, new_item_name);
    update_location_col();
    return success;
  } catch (MenuEmpty &e) {
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
    db().delete_location(id);
    update_location_col();
  } catch (MenuEmpty &e) {
    return;
  }
}

void LocationsScreen::toggle_active_item() {
  try {
    auto id = location_col->get_current_id();
    db().toggle_location_active(id);
    update_location_col();
  } catch (MenuEmpty &e) {
    return;
  }
}

void LocationsScreen::toggle_archive_visibility() {
  show_only_active = !show_only_active;
  update_location_col();
}
