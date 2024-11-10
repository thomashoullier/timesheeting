/** @file Loaded key bindings. */
#ifndef BOUND_KEYS_H
#define BOUND_KEYS_H

#include "data_objects/key.h"

class BoundKeys {
public:
  const KeyBindings kb;

  static BoundKeys &get(const KeyBindings *_kb = nullptr);

private:
  BoundKeys(const KeyBindings &_kb);
};

#endif // BOUND_KEYS_H
