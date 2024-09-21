/** @file
 * @brief GenericItem definition. */
#ifndef GENERIC_ITEM_H
#define GENERIC_ITEM_H

#include <string>
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

#endif // GENERIC_ITEM_H
