/** @file
 * @brief GenericItem definition. */
#ifndef GENERIC_ITEM_H
#define GENERIC_ITEM_H

#include <string>
#include <type_traits>
#include <vector>
#include "id.h"

/** @brief A generic item has an Id and a display name. */
struct GenericItem {
  /** @brief Globally unique Id. */
  Id id;
  /** @brief Name of the item. */
  std::string name;
};

/** @brief Specialization of GenericItem into a Project item. */
struct Project : GenericItem {};
/** @brief Specialization of GenericItem into a Task item. */
struct Task : GenericItem {};
/** @brief Specialization of GenericItem into a Location item. */
struct Location : GenericItem {};

/** @brief Convert a set of generic items into the set of their names. */
template <typename T,
  typename = std::enable_if_t<std::is_base_of_v<GenericItem, T>>>
std::vector<std::string>
generic_item_names(const std::vector<T> &items) {
  std::vector<std::string> names;
  names.reserve(items.size());
  for (const auto &it : items) {
    names.push_back(it.name);
  }
  return names;
};

#endif // GENERIC_ITEM_H
