#ifndef BINDING_MAPS_H
#define BINDING_MAPS_H

#include "key.h"
#include <stdexcept>
#include <unordered_map>

namespace config {
  /** @brief Map from binding to action. */
  template <typename T> class BindingMap {
  private:
    /** @brief Underlying hashmap representation. */
    std::unordered_map<int, T> map;

  public:
    /** @brief Initialize a new map. */
    BindingMap() = default;

    /** @brief Add a new binding to the map. */
    void add_binding(int binding, T action) {
      if (map.contains(binding)) {
        throw std::runtime_error("Tried to map an existing binding!");
      } else {
        map.insert({binding, action});
      }
    };
    /** @brief Retrieve the action corresponding to the key pressed. */
    T action_requested(int binding) const {
      if (map.contains(binding)) {
        return map.at(binding);
      } else {
        return T::unbound;
      }
    };
  };

  /** @brief Map for the normal mode. */
  typedef BindingMap<NormalActions> NormalMap;
  /** @brief Map for the edit mode. */
  typedef BindingMap<EditActions> EditMap;

  /** @brief Represents the key bindings in the program. */
  struct KeyBindings {
    /** @brief The normal mode key map. */
    NormalMap normal_mode;
    /** @brief The edit mode key map. */
    EditMap edit_mode;
  };

} // namespace config

#endif // BINDING_MAPS_H
