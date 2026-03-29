#ifndef PROGRAM_H
#define PROGRAM_H

#include "project.h"
#include "user.h"

/**
 * holds the global state of the application, including pointers to all data lists,
 * the currently logged-in user, and the active sorting strategy for user displays.
 */
typedef struct {
  UserCredentialList *credential_list;
  UserList *user_list;
  ProjectList *project_list;
  MaterialList *material_list;

  User *user;

  int (*UserSortStrategy)(const User*, const User*);
} ProgramState;

/**
 * prompts for credentials and attempts to authenticate a user.
 * @param state pointer to the program state.
 * @return 1 if login successful, 0 otherwise.
 */
int handle_login(ProgramState *state);

/**
 * manages the password recovery flow using security questions.
 * @param state pointer to the program state.
 * @return 1 if password was successfully reset, 0 otherwise.
 */
int handle_forgot_password(ProgramState *state);

/**
 * prints a formatted table of all users in the system.
 * applies the active sorting strategy if one is set.
 * @param state pointer to the program state.
 */
void handle_display_users(ProgramState *state);

/**
 * prompts for new user details and adds them to the system.
 * @param state pointer to the program state.
 * @return 1 if registration successful, 0 otherwise.
 */
int handle_register_user(ProgramState *state);

/**
 * modifies the name components of an existing user profile.
 * @param state pointer to the program state.
 * @return 1 if update successful.
 */
int handle_edit_profile(ProgramState *state);

/**
 * modifies the password and security answer for an existing account.
 * @param state pointer to the program state.
 * @return 1 if update successful, 0 otherwise.
 */
int handle_edit_credentials(ProgramState *state);

/**
 * removes a user and their credentials from the system.
 * prevents a user from deleting their own active session.
 * @param state pointer to the program state.
 * @return 1 if deletion successful, 0 otherwise.
 */
int handle_delete_user(ProgramState *state);

/**
 * prints a summary list of all projects and their carbon status.
 * @param state pointer to the program state.
 */
void handle_display_projects(ProgramState *state);

/**
 * prompts for project details and adds a new project to the list.
 * @param state pointer to the program state.
 * @return 1 if successful.
 */
int handle_add_project(ProgramState *state);

/**
 * removes a project from the system by id.
 * @param state pointer to the program state.
 * @return 1 if successful, 0 otherwise.
 */
int handle_delete_project(ProgramState *state);

/**
 * placeholder for viewing specific project details via user input.
 * @param state pointer to the program state.
 * @return status code.
 */
int handle_view_project(ProgramState *state);

/**
 * prints a detailed breakdown of a project, its sections, and carbon analysis.
 * @param state pointer to the program state.
 * @param project pointer to the specific project to display.
 */
void handle_display_project(ProgramState *state, Project *project);

/**
 * prints the full catalog of available materials.
 * @param state pointer to the program state.
 */
void handle_display_materials(ProgramState *state);

/**
 * adds a new material to the global catalog.
 * @param state pointer to the program state.
 * @return 1 if successful.
 */
int handle_add_material(ProgramState *state);

/**
 * removes a material from the catalog by id.
 * @param state pointer to the program state.
 * @return 1 if successful, 0 otherwise.
 */
int handle_delete_material(ProgramState *state);

#endif // PROGRAM_H
