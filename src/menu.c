#include "menu.h"
#include "io_util.h"
#include "list_util.h"

int run_menu_internal(const Menu *menu, ProgramState *state, int *is_active) {
  List(int) valid_indices = EMPTY_LIST;
  for (int i = 0; i < menu->entries.size; i++) {
    if ((state->user->permission &
         menu->entries.items[i].required_permission) ==
        menu->entries.items[i].required_permission) {
      list_push(&valid_indices, i);
    }
  }

  io_print_header(menu->title);
  printf(BRED "\n [0] " RESET WHT "Back/Exit\n" RESET);

  for (int i = 0; i < valid_indices.size; i++) {
    printf(BGRN " [%d] " RESET WHT "%s\n" RESET, i + 1,
           menu->entries.items[valid_indices.items[i]].label);
  }

  printf("\n");
  io_print_input("Selection");

  int choice;
  if (scanf("%d", &choice) != 1) {
    io_print_error("Invalid input type");
    io_clear_buffer();
    list_free(&valid_indices);
    return -1; 
  }
  io_clear_buffer();

  if (choice == 0) {
    *is_active = 0;
    list_free(&valid_indices);
    return -1;
  }

  if (choice > 0 && choice <= valid_indices.size) {
    int actual_idx = valid_indices.items[choice - 1];
    list_free(&valid_indices);
    return actual_idx;
  }

  io_print_error("Invalid Choice.");
  list_free(&valid_indices);
  return -1;
}
