#ifndef MENU_H
#define MENU_H

#include "program.h"

typedef struct {
    const char *label;
    unsigned int required_permission;
} MenuItem;

typedef struct {
    const char *title;
    List(MenuItem) entries;
} Menu;

int run_menu_internal(const Menu *menu, ProgramState *state, int *is_active);

#define run_menu(menu_ptr, state_ptr, out_idx)                                 \
  for (int _active = 1, out_idx = -1; _active;)                                \
    if ((out_idx = run_menu_internal(menu_ptr, state_ptr, &_active)) == -1) {  \
    } else

#endif
