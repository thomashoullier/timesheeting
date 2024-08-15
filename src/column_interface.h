/** @file
 * @brief Generic GUI column interface and utilities. */
#ifndef COLUMN_INTERFACE_H
#define COLUMN_INTERFACE_H

#include "data_objects.h"
#include <exception>
#include <vector>

/** @brief Column position on the screen. */
enum ColPos { left = 0, middle = 1, right = 2 };

/** @brief Interface for a column holding items. Part which does not depend on
    the item type. */
class ColumnInterfaceBase {
public:
  virtual ~ColumnInterfaceBase () = default;
  /** @brief Get the Id for the currently selected item. */
  virtual Id get_current_id() = 0;
  /** @brief Get the name for the currently selected item and empty string if no
      items are selected. */
  virtual std::string get_current_name() = 0;
  /** @brief Select the next item. */
  virtual void select_next_item() = 0;
  /** @brief Select the previous item. */
  virtual void select_prev_item() = 0;
  /** @brief Query a user input character and pass it. */
  virtual char query_input() = 0;
  /** @brief Query a name for a new item. Empty on cancellation. */
  virtual std::string query_new_item_name() = 0;
  /** @brief Query a new name for the current item. */
  virtual std::string query_current_item_rename () = 0;
  /** @brief Refresh the display. */
  virtual void refresh () = 0;
  /** @brief Clear the display. */
  virtual void clear () = 0;

protected:
  /** @brief Definition of the intended column position. */
  ColPos pos;
};

/** @brief Interface for a column holding a set of GenericItem, navigating
    and querying user inputs. */
template <typename T,
          typename = std::enable_if_t<std::is_base_of<GenericItem, T>::value>>
class ColumnInterface : public ColumnInterfaceBase {
public:
  virtual ~ColumnInterface () = default;
  // TODO: no virtual constructor, how do we constrain the constructor?
  // /** Create a column at the given position. */
  // ColumnInterface(const std::vector<T> &items, ColPos pos) = 0;
  /** @brief Set or refresh the current list of items held by the column. */
  virtual void set_items(const std::vector<T> &items) = 0;

};

/** @brief Exception when encountering an empty column when running an operation
    which requires items. */
class ColumnEmpty : public std::exception {
private:
  /** @brief Exception message. */
  std::string msg;

public:
  ColumnEmpty (const char* _msg) : msg(_msg) {}
  const char* what() const throw() {
    return msg.c_str();
  }
};

#endif // COLUMN_INTERFACE_H
