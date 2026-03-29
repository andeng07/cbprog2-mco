#include "program.h"

#include "io_util.h"
#include "list_util.h"
#include "project.h"
#include "string_util.h"

int handle_login(ProgramState *state) {
  unsigned int user_id;
  String32 user_password;

  io_get_uint("Enter User ID", &user_id);
  io_get_string("Enter Password", user_password, sizeof(String32));

  User *authorized_user = login_user(state->credential_list, state->user_list,
                                     user_id, user_password);

  if (!authorized_user) {
    io_print_error("Login Failed: Invalid Credentials.");
    return 0;
  }

  state->user = authorized_user;

  io_print_success("Login Successfull. Welcome Back!");

  return 1;
}

int handle_forgot_password(ProgramState *state) {
  unsigned int user_id;
  io_get_uint("Enter User ID", &user_id);

  if (!get_cred_by_id(state->credential_list, user_id)) {
    io_print_error("Forgot Password Failed: User Not Found.");
    return 0;
  }

  String32 security_answer;

  io_get_string("Enter Security Answer", security_answer, sizeof(String32));

  UserCredential *user_credential =
      verify_security_answer(state->credential_list, user_id, security_answer);

  if (!user_credential) {
    io_print_error("Forgot Password Failed: Wrong Verification Answer.");
    return 0;
  }

  String32 new_password;

  io_get_string("Enter New Password", new_password, 33);

  update_credential(state->credential_list, user_credential, new_password,
                    NULL);

  io_print_success("Change Password Successful: Please proceed to login.");

  return 1;
}

void handle_display_users(ProgramState *state) {
  // if (!state || !state->user_list)
  //   return 0;

  if (state->UserSortStrategy != NULL)
    list_sort(state->user_list, state->UserSortStrategy);

  printf(GRN "+------+--------------+------------------------------------------"
             "----------+\n");

  printf("| " BGRN "%-4s" RESET GRN " | " BGRN "%-12s" RESET GRN " | " BGRN
         "%-50s" RESET GRN " |\n",
         "ID", "Permissions", "Full Name");

  printf("+------+--------------+----------------------------------------------"
         "------+\n" RESET);

  list_foreach(User, u, state->user_list) {
    const char *role =
        (u->permission == 1) ? "Admin" : "User"; // TODO: MAP THIS
    const char *role_color = (u->permission == 1) ? BWHT : WHT;

    char full_name[128];
    snprintf(full_name, sizeof(full_name), "%s %s %s", u->name.first_name,
             u->name.middle_name, u->name.last_name);

    printf(GRN "| " CYN "%-4u" RESET GRN " | %s%-12s " RESET GRN "| " WHT
               "%-50s" RESET GRN " |\n",
           u->id, role_color, role, full_name);
  }

  printf("+------+--------------+----------------------------------------------"
         "------+\n" RESET);

  // return 1;
}

int handle_register_user(ProgramState *state) {
  User new_user;
  UserCredential new_cred;

  io_print_header("Register New User");

  io_get_string("First Name", new_user.name.first_name, sizeof(String32));
  io_get_string("Middle Name", new_user.name.middle_name, sizeof(String32));
  io_get_string("Last Name", new_user.name.last_name, sizeof(String32));

  unsigned int role;
  io_get_uint("Role (1:User, 2:Project Manager, 3:Admin)", &role);

  switch (role) {
  case 1:
    new_user.permission = USER_PERMS;
    break;
  case 2:
    new_user.permission = PROJECT_MANAGER_PERMS;
    break;
  case 3:
    new_user.permission = ADMIN_PERMS;
    break;
  default:
    io_print_error("Invalid role.");
    return 0;
    break;
  }

  io_get_string("Set Password", new_cred.password, sizeof(String32));
  io_get_string("Security Question: Favorite Color?", new_cred.recovery_answer,
                sizeof(String32));

  if (register_user(state->credential_list, state->user_list, &new_cred,
                    &new_user)) {
    save_users_to_file(state->user_list);
    save_creds_to_file(state->credential_list);
    io_print_success("User registered successfully.");
    return 1;
  }

  return 0;
}

int handle_edit_profile(ProgramState *state) {
  io_print_header("Edit Profile");

  unsigned int target;

  io_get_uint("Enter account id", &target);

  User *u = get_user_by_id(state->user_list, target);

  printf("Current Name: %s %s %s\n", u->name.first_name, u->name.middle_name,
         u->name.last_name);

  io_get_string("New First Name", u->name.first_name, 32);
  io_get_string("New Middle Name", u->name.middle_name, 32);
  io_get_string("New Last Name", u->name.last_name, 32);

  save_users_to_file(state->user_list);
  io_print_success("Profile updated.");
  return 1;
}

int handle_edit_credentials(ProgramState *state) {
  io_print_header("Edit Credential");

  unsigned int target;

  io_get_uint("Enter account id", &target);

  UserCredential *cred = get_cred_by_id(state->credential_list, target);

  if (!cred) {
    io_print_error("Credentials not found.");
    return 0;
  }

  String32 new_pass, new_ans;
  io_get_string("New Password", new_pass, sizeof(String32));
  io_get_string("New Security Answer", new_ans, sizeof(String32));

  if (!update_credential(state->credential_list, cred, new_pass, new_ans))
    return 0;

  io_print_success("Credentials updated successfully.");
  return 1;
}

int handle_delete_user(ProgramState *state) {
  io_print_header("Edit Delete Account");

  unsigned int target;

  io_get_uint("Enter account id", &target);

  if (target == state->user->id) {
    io_print_error("You can't delete an active account");
    return 0;
  }

  UserCredential *cred = get_cred_by_id(state->credential_list, target);
  User *u = get_user_by_id(state->user_list, target);

  if (!cred || !u)
    return 0;

  if (!delete_user(state->credential_list, state->user_list, cred, u))
    return 0;

  io_print_success("Account has been deleted successfully.");
  return 1;
}

void handle_display_materials(ProgramState *state) {
  io_print_header("Material Catalog");
  printf(GRN "| %-4s | %-32s | %-16s | %-32s | %-32s | %-8s | %-8s |\n" RESET,
         "ID", "Company", "Category", "Type", "Product", "Carbon", "Density");

  printf(
      "------------------------------------------------------------------------"
      "------------------------------------------------------------------------"
      "--------------------------\n");

  list_foreach(Material, m, state->material_list) {
    printf("| %-4u | %-32s | %-16s | %-32s | %-32s | %-8.2f | %-8.2f |\n",
           m->id, m->company_name, m->category, m->type, m->product_name,
           m->embodied_carbon, m->density);
  }
  printf(
      "------------------------------------------------------------------------"
      "------------------------------------------------------------------------"
      "--------------------------\n");
}

int handle_add_material(ProgramState *state) {
  Material m = {0};
  io_print_header("Add New Material");
  io_get_uint("ID", &m.id);
  io_get_string("Company", m.company_name, 64);
  io_get_string("Category", m.category, 64);
  io_get_string("Type", m.type, 64);
  io_get_string("Product Name", m.product_name, 64);
  io_get_float("Embodied Carbon (KgCO2e/unit)", &m.embodied_carbon);

  list_push(state->material_list, m);
  save_materials(state->material_list);
  io_print_success("Material added successfully.");
  return 1;
}

int handle_delete_material(ProgramState *state) {
  unsigned int id;
  io_get_uint("Enter Material ID to delete", &id);
  if (!delete_material(state->material_list, id)) {
    io_print_error("Deleting material failed.");
    return 0;
  }
  io_print_success("Material removed.");
  return 1;
}

void handle_display_projects(ProgramState *state) {
  // TODO:
  io_print_header("Project List");

  printf(GRN "| %-4s | %-28s | %-8s | %-17s | %-9s |\n" RESET, "ID",
         "Project Name", "Sections", "Embodied Emission", "Status");

  list_foreach(Project, p, state->project_list) {
    float project_ee = calculate_project_ee(p);
    float project_ee_ceil = calculate_project_ee_ceiling(state->material_list, p);

    printf("| %-4u | %-28s | %-8d | %-17.2f | %-9s |" RESET "\n", p->id, p->project_name,
           p->sections.size, project_ee, project_ee <= project_ee_ceil ? "Safe" : "Danger");
  }
}

int handle_add_project(ProgramState *state) {
  Project p = {.sections = EMPTY_LIST}; // Use your macro for the internal list
  io_print_header("Create New Project");
  io_get_uint("Project ID", &p.id);
  io_get_string("Project Name", p.project_name, 64);

  list_push(state->project_list, p);
  save_projects(state->project_list);

  return 1;
}

void handle_display_project(ProgramState *state, Project *project) {
  char header_buffer[128];

  // 1. Centered Header Card
  snprintf(header_buffer, sizeof(header_buffer), "PROJECT: %s (ID: %u)",
           project->project_name, project->id);
  printf("\n\n");
  float project_ee = calculate_project_ee(project);
  float project_ee_ceil =
      calculate_project_ee_ceiling(state->material_list, project);

  printf("Total Embodied Emission: %.2f/%.2f %s", project_ee, project_ee_ceil,
         project_ee <= project_ee_ceil ? GRN "[Normal]" : RED "[Danger]");
  printf("\n\n");
  io_print_centered(header_buffer, BGRN);
  printf("\n");

  // 2. Table Headers
  // Using a wider layout to accommodate the extra data
  printf("  %-4s | %-15s | %-32s | %-10s | %-12s\n", "ID", "Section",
         "Material", "Vol (m3)", "Embodied Em.");
  printf(
      "  "
      "-----|-----------------|----------------------------------|------------|-------------\n");

  for (int i = 0; i < project->sections.size; i++) {
    Section *s = &project->sections.items[i];

    double volume = s->length * s->width * s->height;

    double embodied = calculate_embodied_carbon(s);

    printf("  %-4d | %-15s | %-32s | %-10.2f | %-12.2f\n", i + 1,
           s->section_name, s->material.product_name, volume, embodied);
  }

  // 3. Footer Summary
  printf("  "
         "----------------------------------------------------------------------------"
         "---------\n");
  printf("  " GRN "TOTAL EMBODIED CARBON:" RESET " %10.2f kg CO2e\n\n",
         project_ee);

  // Centered Footer
  io_print_centered("END OF PROJECT REPORT", BWHT);
  printf("\n");
}
