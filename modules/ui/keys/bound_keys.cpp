#include "bound_keys.h"

namespace keys {
  BoundKeys &BoundKeys::get(const KeyBindings *_kb) {
    static BoundKeys instance{*_kb};
    return instance;
  }

  BoundKeys::BoundKeys(const KeyBindings &_kb) : kb{_kb} {}
}
