/** @file
 *  @brief LocationsScreen definition. */
#ifndef LOCATIONS_SCREEN_H
#define LOCATIONS_SCREEN_H

#include "column/column.h"
#include "config/key.h"
#include "ui_screen.h"
#include <memory>

namespace tui {
  /** @brief Locations UI screen. */
  class LocationsScreen final : public UIScreen {
  public:
    /** @brief Constructor. */
    explicit LocationsScreen();

    void refresh () final;
    void clear () override;
    void resize () final;
    void update () override;
    void update_status() override;
    config::NormalActions input_loop () override;

  private:
    /** @brief Column holding the locations. */
    std::unique_ptr<Column<core::Location>> location_col;
    /** @brief Whether to show only active items. */
    bool show_only_active;

    /** @brief Update the location column. */
    void update_location_col();
    /** @brief Add an item to the column. */
    bool add_item ();
    /** @brief Rename an item in the column. */
    bool rename_item ();
    /** @brief Remove an item in the column. */
    void remove_item();
    /** @brief Toggle the active flag on the selected item. */
    void toggle_active_item ();
    /** @brief Toggle the archive items visibility. */
    void toggle_archive_visibility();
  };
} // namespace tui

#endif // LOCATIONS_SCREEN_H
