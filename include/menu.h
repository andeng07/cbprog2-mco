#ifndef MENU_H
#define MENU_H

#include "program.h"

/**
 * represents a single option within a menu, tied to a specific permission bit.
 */
typedef struct {
  const char *label;
  unsigned int required_permission; /**< permission bitmask needed to see/access this item */
} MenuItem;

/**
 * menu
 * a collection of menu items with a descriptive title.
 */
typedef struct {
    const char *title;
    List(MenuItem) entries;
} Menu;

/**
 * displays the menu, filters items by user permissions, and handles user input.
 * @param menu pointer to the menu to display.
 * @param state pointer to the current program state (used for permission checks).
 * @param is_active pointer to a flag used to control the calling loop's lifecycle.
 * @return the index of the selected menu item, or -1 if no valid selection was made.
 */
int run_menu_internal(const Menu *menu, ProgramState *state, int *is_active);

/**
 * run_menu
 * abstraction macro to wrap the menu logic in a loop.
 * it continues to run until the user selects '0' (exit/back) or a valid option is chosen.
 * @param menu_ptr pointer to the menu struct.
 * @param state_ptr pointer to the program state struct.
 * @param out_idx variable name to store the resulting selected index.
 */
#define run_menu(menu_ptr, state_ptr, out_idx)                                 \
  for (int _active = 1, out_idx = -1; _active;)                                \
    if ((out_idx = run_menu_internal(menu_ptr, state_ptr, &_active)) == -1) {  \
    } else

#endif // MENU_H
