/** @file
 *  @brief Loaded key bindings. */
#ifndef BOUND_KEYS_H
#define BOUND_KEYS_H

#include "config/binding_maps.h"

namespace keys {
  /** @brief Accessor for the key bindings.

   Singleton pattern. */
  class BoundKeys {
  public:
    /** @brief Loaded set of key bindings. */
    const config::KeyBindings kb;

    /** @brief Singleton grabber. */
    static BoundKeys &get(const config::KeyBindings *_kb = nullptr);

    BoundKeys(BoundKeys const&) = delete;
    void operator=(BoundKeys const&) = delete;

  private:
    /** @brief Initialize the singleton fomr the key bindings read from the
               configuration. */
    explicit BoundKeys(const config::KeyBindings &_kb);
  };
}
#endif // BOUND_KEYS_H
