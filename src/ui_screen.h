#ifndef UI_SCREEN_H
#define UI_SCREEN_H

/** @brief Interface for a UI screen. */
class UIScreen {
public:
  virtual ~UIScreen () = default;
  /** @brief Update the screen information and display it. */
  virtual void refresh () = 0;
  /** @brief Clear the screen from the display. */
  virtual void clear () = 0;
  /** @brief UI input loop for the screen. */
  virtual char input_loop () = 0;
};

#endif // UI_SCREEN_H
