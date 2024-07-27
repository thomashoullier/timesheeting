#ifndef COLUMN_INTERFACE_H
#define COLUMN_INTERFACE_H

#include "data_objects.h"
#include <vector>

/** Column position on the screen. */
enum ColPos { left = 0, middle = 1, right = 2 };

/** Interface for a column holding items. Part which does not
    depend on the item type. */
class ColumnInterfaceBase {
public:
  /** Get the Id for the currently selected item. */
  virtual Id get_current_id() = 0;
  /** Select the next item. */
  virtual void select_next_item() = 0;
  /** Select the previous item. */
  virtual void select_prev_item() = 0;
  /** Query a user input character and pass it. */
  virtual char query_input() = 0;
  /** Query a name for a new item. Empty on cancellation. */
  virtual std::string query_new_item_name() = 0;
  /** Query a new name for the current item. */
  virtual std::string query_current_item_rename () = 0;
  /** Refresh the display. */
  virtual void refresh () = 0;

protected:
  /** Definition of the intended column position. */
  ColPos pos;
};

/** Interface for a column holding a set of GenericItem, navigating
    and querying user inputs. */
template <typename T,
          typename = std::enable_if_t<std::is_base_of<GenericItem, T>::value>>
class ColumnInterface : public ColumnInterfaceBase {
public:
  // TODO: no virtual constructor, how do we constrain the constructor?
  // /** Create a column at the given position. */
  // ColumnInterface(const std::vector<T> &items, ColPos pos) = 0;
  /** Set or refresh the current list of items held by the column. */
  virtual void set_items(const std::vector<T> &items) = 0;

};

#endif // COLUMN_INTERFACE_H
