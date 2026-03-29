#include "io_util.h"
#include "program.h"
#include "user.h"
#include <stdio.h>

// Define constants for better readability
#define CHOICE_LOGIN 1
#define CHOICE_FORGOT_PASSWORD 2
#define CHOICE_EXIT 0

void initial_menu(ProgramState *state);
void main_view(ProgramState *state) {
  int perm = state->user->permission;

  printf("--- Welcome, %s ---\n", state->user->name.first_name);

  // Level 1 & 3 see everything; Level 2 is filtered
  if (perm == 2) {
    printf("Viewing: Your Personal Projects\n");
    // list_projects_filtered(state->user->id);
  } else {
    printf("Viewing: All Global Projects\n");
    // list_all_projects();
  }

  // Conditional Menu Options based on "Extra" permissions
  printf("\n[Actions]\n");
  printf("1. View Project Details\n");

  if (perm >= 3) {
    printf("2. Edit/Delete Projects (Admin)\n");
  }

  if (perm == 4) {
    printf("3. System Maintenance (App Admin)\n");
  }

  printf("0. Logout\n");
}

void action_login(ProgramState *state);
void action_forgot_password(ProgramState *state);

int main() {
  ProgramState state;
  UserCredentialList credential_list = EMPTY_LIST;
  UserList user_list = EMPTY_LIST;

  load_creds_from_file(&credential_list);
  load_users_from_file(&user_list);

  printf("credential_list size: %d\n", credential_list.size);
  printf("user_list size: %d\n", user_list.size);

  User user;
  state.credential_list = &credential_list;
  state.user_list = &user_list;
  state.user = &user;

  // Start the menu loop
  initial_menu(&state);

  return 0;
}

void main_menu(ProgramState *state) {
  int choice = -1;

  while (choice != CHOICE_EXIT) {
    io_print_header("EMBODIED EMISSIONS");

    printf("\n");

    printf("[0] Exit\n");
    printf("[1] Login\n");
    printf("[2] Forgot Password\n");

    printf("\n");

    printf("Enter choice: ");

    // Basic input validation
    if (scanf("%d", &choice) != 1) {
      printf("Invalid input. Please enter a number.\n");
      io_clear_buffer();
      continue;
    }

    printf("\n");

    switch (choice) {
    case CHOICE_LOGIN:
      action_login(state);
      break;
    case CHOICE_FORGOT_PASSWORD:
      action_forgot_password(state);
      break;
    case CHOICE_EXIT:
      printf("Exiting system...\n");
      break;
    default:
      printf("Invalid selection. Try again.\n");
      break;
    }
  }
}

void action_login(ProgramState *state) {
  if (handle_login(state)) {
    // main menu

    return;
  }

  io_pause();
}

void action_forgot_password(ProgramState *state) {
  if (handle_forgot_password(state)) {
    io_pause();

    return;
  }
}
