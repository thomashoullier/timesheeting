/** @file
 * @brief ColumnBase definition. */
#ifndef COLUMN_BASE_H
#define COLUMN_BASE_H

#include "core/id.h"
#include <string>

namespace tui {
  /** @brief Class interface for manipulating a column regardless
      of its type. */
  class ColumnBase {
  public:
    virtual ~ColumnBase () = default;
    /** @brief Get the Id for the currently selected item. */
    virtual core::Id get_current_id() = 0;
    /** @brief Get the name for the currently selected item and
        empty string if no items are selected. */
    virtual std::string get_current_item_string() = 0;
    /** @brief Select the next item. */
    virtual void select_down_item() = 0;
    /** @brief Select the previous item. */
    virtual void select_up_item() = 0;
    /** @brief Query a user input character and pass it. */
    virtual int get_input() = 0;
    /** @brief Set the border for highlighting. */
    virtual void set_border() = 0;
    /** @brief Unset the border. */
    virtual void unset_border() = 0;
  };
} // namespace tui

#endif // COLUMN_BASE_H
