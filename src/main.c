#include "io_util.h"
#include "list_util.h"
#include "menu.h"
#include "program.h"
#include "project.h"
#include "user.h"

void open_auth_menu(ProgramState *state);
void open_main_menu(ProgramState *state);
void open_acc_menu(ProgramState *state);
void open_acc_sort_strategy_menu(ProgramState *state);
void open_material_menu(ProgramState *state);
void open_project_menu(ProgramState *state);
void open_project_view_menu(ProgramState *state, Project *p);

int main() {
  ProgramState state;
  UserCredentialList credential_list = EMPTY_LIST;
  UserList user_list = EMPTY_LIST;

  load_creds_from_file(&credential_list);
  load_users_from_file(&user_list);

  MaterialList material_list = EMPTY_LIST;
  ProjectList project_list = EMPTY_LIST;

  load_materials(&material_list);
  load_projects(&project_list, &material_list);

  User user;
  state.credential_list = &credential_list;
  state.user_list = &user_list;
  state.material_list = &material_list;
  state.project_list = &project_list;
  state.user = &user;

  print_title();
  open_auth_menu(&state);

  list_free(&credential_list);
  list_free(&user_list);
  return 0;
}

// --- AUTH MENU ---
void open_auth_menu(ProgramState *state) {
  Menu menu = {.title = "Authentication", .entries = EMPTY_LIST};
  list_push(&menu.entries, ((MenuItem){"Login", 0}));
  list_push(&menu.entries, ((MenuItem){"Forgot Password", 0}));

  run_menu(&menu, state, selection) {
    switch (selection) {
    case 0:
      if (handle_login(state)) {
        io_pause();
        open_main_menu(state);
      }
      break;
    case 1:
      if (handle_forgot_password(state))
        io_pause();
      break;
    }
  }

  list_free(&menu.entries);
}

// --- MAIN MENU ---
void open_main_menu(ProgramState *state) {
  Menu menu = {.title = "Main Menu", .entries = EMPTY_LIST};
  list_push(&menu.entries, ((MenuItem){"Materials", USER_PERMS}));
  list_push(&menu.entries, ((MenuItem){"Projects", USER_PERMS}));
  list_push(&menu.entries, ((MenuItem){"Accounts", ADMIN_PERMS}));

  run_menu(&menu, state, selection) {
    if (selection == 0)
      open_material_menu(state);
    if (selection == 1) /* Project logic */
      open_project_menu(state);
    if (selection == 2)
      open_acc_menu(state);
  }
  list_free(&menu.entries);
}

// --- ACCOUNT MENU ---
void open_acc_menu(ProgramState *state) {
  Menu menu = {.title = "Account Management", .entries = EMPTY_LIST};
  list_push(&menu.entries, ((MenuItem){"List Users", PERM_ACCOUNT_LIST}));
  list_push(&menu.entries,
            ((MenuItem){"Register New User", PERM_ACCOUNT_REGISTER}));
  list_push(&menu.entries, ((MenuItem){"Edit User", PERM_ACCOUNT_EDIT_USER}));
  list_push(&menu.entries,
            ((MenuItem){"Edit Credentials", PERM_ACCOUNT_EDIT_CRED}));
  list_push(&menu.entries, ((MenuItem){"Delete", PERM_ACCOUNT_DELETE}));

  run_menu(&menu, state, selection) {
    switch (selection) {
    case 0:
      open_acc_sort_strategy_menu(state);
      break;
    case 1:
      if (handle_register_user(state))
        io_pause();
      break;
    case 2:
      if (handle_edit_profile(state))
        io_pause();
      break;
    case 3:
      if (handle_edit_credentials(state))
        io_pause();
      break;
    case 4:
      if (handle_delete_user(state))
        io_pause();
      break;
    }
  }
  list_free(&menu.entries);
}

// --- SORT STRATEGY MENU (The biggest cleanup!) ---

int cmp_id_asc(const User *a, const User *b) {
  return user_comparator(a, b, 1, 1);
}
int cmp_id_desc(const User *a, const User *b) {
  return user_comparator(a, b, 1, -1);
}
int cmp_perm_asc(const User *a, const User *b) {
  return user_comparator(a, b, 2, 1);
}
int cmp_perm_desc(const User *a, const User *b) {
  return user_comparator(a, b, 2, -1);
}
int cmp_name_asc(const User *a, const User *b) {
  return user_comparator(a, b, 3, 1);
}
int cmp_name_desc(const User *a, const User *b) {
  return user_comparator(a, b, 3, -1);
}

void open_acc_sort_strategy_menu(ProgramState *state) {
  Menu menu = {.title = "Sort Configuration", .entries = EMPTY_LIST};
  list_push(&menu.entries, ((MenuItem){"ID: Low to High", PERM_ACCOUNT_LIST}));
  list_push(&menu.entries, ((MenuItem){"ID: High to Low", PERM_ACCOUNT_LIST}));
  list_push(&menu.entries, ((MenuItem){"Name: A-Z", PERM_ACCOUNT_LIST}));
  list_push(&menu.entries, ((MenuItem){"Name: Z-A", PERM_ACCOUNT_LIST}));
  list_push(&menu.entries, ((MenuItem){"Rank: User first", PERM_ACCOUNT_LIST}));
  list_push(&menu.entries,
            ((MenuItem){"Rank: Admin first", PERM_ACCOUNT_LIST}));

  run_menu(&menu, state, selection) {
    switch (selection) {
    case 0:
      state->UserSortStrategy = cmp_id_asc;
      break;
    case 1:
      state->UserSortStrategy = cmp_id_desc;
      break;
    case 2:
      state->UserSortStrategy = cmp_name_asc;
      break;
    case 3:
      state->UserSortStrategy = cmp_name_desc;
      break;
    case 4:
      state->UserSortStrategy = cmp_perm_asc;
      break;
    case 5:
      state->UserSortStrategy = cmp_perm_desc;
      break;
    }
    handle_display_users(state);
    io_pause();
  }

  list_free(&menu.entries);
}

void open_project_view_menu(ProgramState *state, Project *p) {
  Menu menu = {.title = "Project Detail View", .entries = EMPTY_LIST};
  list_push(&menu.entries,
            ((MenuItem){"Add Section", PERM_PROJECT_SECTION_ADD}));
  list_push(&menu.entries,
            ((MenuItem){"Delete Section", PERM_PROJECT_SECTION_DELETE}));

  handle_display_project(state, p);

  run_menu(&menu, state, selection) {
    switch (selection) {
    case 0: {
      Section s = {0};
      unsigned int mat_id;
      io_get_string("Section Name", s.section_name, 64);

      handle_display_materials(state);
      io_get_uint("Select Material ID", &mat_id);

      Material *found = get_material_by_id(state->material_list, mat_id);

      if (found) {
        s.material = *found;
        io_get_float("Length (m)", &s.length);
        io_get_float("Width (m)", &s.width);
        io_get_float("Height (m)", &s.height);

        list_push(&p->sections, s); // Using your macro!
        save_projects(state->project_list);
        io_print_success("Section added.");
      } else {
        io_print_error("Material ID not found.");
      }
      break;
    }
    case 1: {
      int idx;
      io_get_int("Enter section number to delete", &idx);
      list_remove_at(&p->sections, idx - 1);
      save_projects(state->project_list);
      io_print_success("Section removed.");
      break;
    }
    }

    handle_display_project(state, p);
  }
  list_free(&menu.entries);
}

// --- MATERIAL MENU ---
void open_material_menu(ProgramState *state) {
  Menu menu = {.title = "Material Management", .entries = EMPTY_LIST};
  list_push(&menu.entries, ((MenuItem){"List Materials", PERM_MATERIAL_LIST}));
  list_push(&menu.entries, ((MenuItem){"Add Material", PERM_MATERIAL_ADD}));
  list_push(&menu.entries,
            ((MenuItem){"Delete Material", PERM_MATERIAL_DELETE}));

  run_menu(&menu, state, selection) {
    switch (selection) {
    case 0:
      handle_display_materials(state);
      io_pause();
      break;
    case 1:
      handle_add_material(state);
      break;
    case 2:
      handle_delete_material(state);
      break;
    }
  }
  list_free(&menu.entries);
}

// --- PROJECT MENU ---
void open_project_menu(ProgramState *state) {
  Menu menu = {.title = "Project Management", .entries = EMPTY_LIST};
  list_push(&menu.entries, ((MenuItem){"List Projects", PERM_PROJECT_LIST}));
  list_push(&menu.entries, ((MenuItem){"View Project", PERM_PROJECT_VIEW}));
  list_push(&menu.entries, ((MenuItem){"Add Project", PERM_PROJECT_ADD}));
  list_push(&menu.entries, ((MenuItem){"Delete Project", PERM_PROJECT_DELETE}));

  run_menu(&menu, state, selection) {
    switch (selection) {
    case 0:
      handle_display_projects(state);
      io_pause();
      break;
    case 1: {
      unsigned int id;
      io_get_uint("Enter Project ID to View", &id);

      Project *project = get_project_by_id(state->project_list, id);

      if (project)
        open_project_view_menu(state, project);
      else
        io_print_error("Project not found.");

      break;
    }
    case 2:
      if (handle_add_project(state)) {
        open_project_view_menu(state, list_last(state->project_list));
      }
      break;
    case 3: {
      unsigned int id;
      io_get_uint("Enter Project ID to Delete", &id);
      delete_project(state->project_list, id);
      break;
    }
    }
  }
  list_free(&menu.entries);
}
