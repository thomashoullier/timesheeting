#ifndef LOCATIONS_SCREEN_H
#define LOCATIONS_SCREEN_H

#include "column/column.h"
#include "status_bar/status_bar_ncurses.h"
#include "ui_component.h"
#include "../db/db_sqlite.h"
#include "../logger/logger.h"
#include <memory>

/** @brief Locations UI screen. */
class LocationsScreen : public UIComponent {
public:
  /** @brief Constructor. */
  explicit LocationsScreen()
    : location_col(std::make_unique<Column<Location>>(std::vector<Location>(),
                                                      WindowPosition::left)),
      show_only_active(true) {
    // TODO: initialize the column directly.
    update_location_col();
  };

  void refresh () override { location_col->refresh(); };
  void clear () override { location_col->clear(); };
  void update () override { update_location_col(); };

  char input_loop () override {
    location_col->set_border();
    while(true) {
      status().print(location_col->get_current_item_string());
      auto ch = location_col->get_input();
      switch(ch) {
      case 'n':
        location_col->select_down_item();
        break;
      case 'e':
        location_col->select_up_item();
        break;
      case 'a':
        try {
          add_item();
        }
        catch (DBLogicExcept &e) {
          status().print_wait("DB logic error! Nothing was done to the DB.");
        }
        break;
      case 'r':
        try {
          rename_item();
        }
        catch (DBLogicExcept &e) {
          status().print_wait("DB logic error! Nothing was done to the DB.");
        }
        break;
      case 'x':
        try {
          remove_item();
        }
        catch (DBLogicExcept &e) {
          status().print_wait("DB logic error! Nothing was done to the DB.");
        }
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
  };

private:
  /** @brief Column holding the locations. */
  std::unique_ptr<Column<Location>> location_col;
  /** @brief Whether to show only active items. */
  bool show_only_active;

  /** @brief Update the location column. */
  void update_location_col() {
    std::vector<Location> location_items;
    if (show_only_active)
      location_items = db().query_locations_active();
    else
      location_items = db().query_locations();
    location_col->set_items(location_items);
    location_col->refresh();
  };

  /** @brief Add an item to the column. */
  void add_item () {
    auto new_item_name = status().get_user_string();
    if (new_item_name.empty())
      return;
    db().add_location(new_item_name);
    logger().log("Added location: " + new_item_name);
    update_location_col();
  };

  /** @brief Rename an item in the column. */
  void rename_item () {
    try {
      auto id = location_col->get_current_id();
      auto new_item_name = status().get_user_string();
      if (new_item_name.empty())
        return;
      db().edit_location_name(id, new_item_name);
      update_location_col();
    } catch (MenuEmpty &e) {
      // TODO: instead of managing this by exception, try to check if the column
      // is empty explicitely.
      return;
    }
  };

  /** @brief Remove an item in the column. */
  void remove_item() {
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
  };

  /** @brief Toggle the active flag on the selected item. */
  void toggle_active_item () {
    try {
      auto id = location_col->get_current_id();
      db().toggle_location_active(id);
      update_location_col();
    } catch (MenuEmpty &e) {
      return;
    }
  }

  /** @brief Toggle the archive items visibility. */
  void toggle_archive_visibility() {
    show_only_active = !show_only_active;
    update_location_col();
  }
};

#endif // LOCATIONS_SCREEN_H
