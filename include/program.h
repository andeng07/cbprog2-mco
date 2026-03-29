#ifndef PROGRAM_H
#define PROGRAM_H

#include "user.h"

typedef struct {
  UserCredentialList *credential_list;
  UserList *user_list;

  User *user;
} ProgramState;

int handle_login(ProgramState *state);
int handle_forgot_password(ProgramState *state);

#endif
