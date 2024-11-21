/** @file Loaded key bindings. */
#ifndef BOUND_KEYS_H
#define BOUND_KEYS_H

#include "config/key.h"

namespace keys {
  class BoundKeys {
  public:
    const config::KeyBindings kb;

    static BoundKeys &get(const config::KeyBindings *_kb = nullptr);

  private:
    BoundKeys(const config::KeyBindings &_kb);
  };
}
#endif // BOUND_KEYS_H
