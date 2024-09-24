#ifndef UI_SCREEN_H
#define UI_SCREEN_H

#include "ui_component.h"

class UIScreen : public UIComponent {
protected:
  bool needs_update = false;

public:
  virtual ~UIScreen () = default;

  /** @brief Signal to the object that it needs an update since state has
      changed elsewhere. */
  void signal_update_need();
};

#endif // UI_SCREEN_H
