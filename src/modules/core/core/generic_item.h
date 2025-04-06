/** @file
 * @brief GenericItem definition. */
#ifndef GENERIC_ITEM_H
#define GENERIC_ITEM_H

#include <algorithm>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>
#include "id.h"

namespace core {
  /** @brief A generic item has an Id and a display name. */
  struct GenericItem {
    /** @brief Globally unique Id. */
    Id id;
    /** @brief Name of the item. */
    std::string name;
    /** @brief Is the item active? */
    bool active = true;
  };

  /** @brief Specialization of GenericItem into a Project item. */
  struct Project final : GenericItem {};
  /** @brief Specialization of GenericItem into a Task item. */
  struct Task final : GenericItem {};
  /** @brief Specialization of GenericItem into a Location item. */
  struct Location final : GenericItem {};

  /** @brief Convert a set of generic items into the set of their names. */
  template <typename T,
            typename = std::enable_if_t<std::is_base_of_v<GenericItem, T>>>
  std::vector<std::string>
  generic_item_names(const std::vector<T> &items) {
    std::vector<std::string> names;
    names.reserve(items.size());
    std::transform(items.begin(), items.end(),
                   std::back_inserter(names),
                   [](const auto &it) { return it.name; });
    return names;
  };
}
#endif // GENERIC_ITEM_H
