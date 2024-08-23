/** @file
 * @brief UIComponent definition. */
#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

/** @brief Interface class for a UI component. */
class UIComponent {
public:
  virtual ~UIComponent () = default;
  /** @brief Refresh the component's display. */
  virtual void refresh () = 0;
  /** @brief Clear the component from the display. */
  virtual void clear () = 0;
  /** @brief UI input loop for the component. */
  virtual char input_loop () = 0;
  /** @brief Update the state of the component
      to reflect the latest program state. */
  virtual void update () = 0;
};

#endif // UI_COMPONENT_H
