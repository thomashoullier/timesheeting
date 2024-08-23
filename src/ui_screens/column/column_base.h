/** @file
 * @brief ColumnBase definition. */
#ifndef COLUMN_BASE_H
#define COLUMN_BASE_H

#include "../../data_objects/data_objects.h"

/** @brief Class interface for manipulating a column regardless of its type. */
class ColumnBase {
public:
  virtual ~ColumnBase () = default;
  /** @brief Get the Id for the currently selected item. */
  virtual Id get_current_id() = 0;
  /** @brief Get the name for the currently selected item and empty string if no
      items are selected. */
  virtual std::string get_current_item_string() = 0;
  /** @brief Select the next item. */
  virtual void select_down_item() = 0;
  /** @brief Select the previous item. */
  virtual void select_up_item() = 0;
  /** @brief Query a user input character and pass it. */
  virtual char get_input() = 0;
  /** @brief Query a new name for the current item. */
  virtual std::string query_current_item_rename () = 0;
  /** @brief Query a name for a new item. Empty on cancellation. */
  virtual std::string query_new_item_name() = 0;
};

#endif // COLUMN_BASE_H
