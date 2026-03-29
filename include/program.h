#ifndef PROGRAM_H
#define PROGRAM_H

#include "project.h"
#include "user.h"

typedef struct {
  UserCredentialList *credential_list;
  UserList *user_list;
  ProjectList *project_list;
  MaterialList *material_list;

  User *user;

  int (*UserSortStrategy)(const User*, const User*);
} ProgramState;

int handle_login(ProgramState *state);
int handle_forgot_password(ProgramState *state);

void handle_display_users(ProgramState *state);
int handle_register_user(ProgramState *state);
int handle_edit_profile(ProgramState *state);
int handle_edit_credentials(ProgramState *state);
int handle_delete_user(ProgramState *state);

void handle_display_projects(ProgramState *state);
int handle_add_project(ProgramState *state);
int handle_delete_project(ProgramState *state);
int handle_view_project(ProgramState *state);
void handle_display_project(ProgramState *state, Project *project);

void handle_display_materials(ProgramState *state);
int handle_add_material(ProgramState *state);
int handle_delete_material(ProgramState *state);

#endif
