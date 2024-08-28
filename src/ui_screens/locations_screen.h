#ifndef LOCATIONS_SCREEN_H
#define LOCATIONS_SCREEN_H

#include "column/column.h"
#include "status_bar/status_bar_ncurses.h"
#include "ui_component.h"
#include "../db_interface.h"
#include "../logger/logger.h"
#include <memory>

/** @brief Locations UI screen. */
template <typename T_DB,
          typename =
              std::enable_if_t<std::is_base_of<DB_Interface, T_DB>::value>>
class LocationsScreen : public UIComponent {
public:
  /** @brief Constructor. */
  explicit LocationsScreen(std::shared_ptr<T_DB> _db,
                           std::shared_ptr<StatusBarNCurses> _status)
    : db(std::static_pointer_cast<DB_Interface>(_db)),
      status(_status),
      location_col(std::make_unique<Column<Location>>(std::vector<Location>(),
                                                      WindowPosition::left)) {
    update_location_col();
  };

  void refresh () override { location_col->refresh(); };
  void clear () override { location_col->clear(); };
  void update () override { update_location_col(); };

  char input_loop () override {
    location_col->set_border();
    while(true) {
      status->print(location_col->get_current_item_string());
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
          status->print_wait("DB logic error! Nothing was done to the DB.");
        }
        break;
      case 'r':
        try {
          rename_item();
        }
        catch (DBLogicExcept &e) {
          status->print_wait("DB logic error! Nothing was done to the DB.");
        }
        break;
      case 'x':
        try {
          remove_item();
        }
        catch (DBLogicExcept &e) {
          status->print_wait("DB logic error! Nothing was done to the DB.");
        }
        break;
      default:
        location_col->unset_border();
        return ch;
      }
    }
  };

private:
  /** @brief Interface to the DB. */
  std::shared_ptr<DB_Interface> db;
  /** @brief Interface to the status bar. */
  std::shared_ptr<StatusBarNCurses> status;
  /** @brief Column holding the locations. */
  std::unique_ptr<Column<Location>> location_col;

  /** @brief Update the location column. */
  void update_location_col() {
    auto location_items = db->query_locations();
    location_col->set_items(location_items);
    location_col->refresh();
  };

  /** @brief Add an item to the column. */
  void add_item () {
    auto new_item_name = status->get_user_string();
    if (new_item_name.empty())
      return;
    db->add_location(new_item_name);
    log("Added location: " + new_item_name);
    update_location_col();
  };

  /** @brief Rename an item in the column. */
  void rename_item () {
    try {
      auto id = location_col->get_current_id();
      auto new_item_name = status->get_user_string();
      if (new_item_name.empty())
        return;
      db->edit_location_name(id, new_item_name);
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
      bool user_conf = status->query_confirmation("Remove item? (Y/N)");
      if (!user_conf)
        return;
      db->delete_location(id);
      update_location_col();
    } catch (MenuEmpty &e) {
      return;
    }
  };
};

#endif // LOCATIONS_SCREEN_H
