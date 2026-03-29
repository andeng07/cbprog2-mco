#include "program.h"

#include "io_util.h"
#include "user.h"

int handle_login(ProgramState *state) {
  state->user = NULL;

  unsigned int user_id;
  String32 user_password;

  io_get_int("Enter User ID: ", &user_id);
  io_get_string("Enter Password: ", user_password, 33);

  User *authorized_user = login_user(state->credential_list, state->user_list,
                                     user_id, user_password);

  if (!authorized_user) {
    printf("[!] Login Failed: Invalid Credentials.\n");
    return 0;
  }

  state->user = authorized_user;

  printf("[!] Login Successful: Welcome back, %s, %s!",
         authorized_user->name.last_name, authorized_user->name.first_name);

  return 1;
}

int handle_forgot_password(ProgramState *state) {
  unsigned int user_id;
  io_get_int("Enter User ID: ", &user_id);

  if (!get_cred_by_id(state->credential_list, user_id)) {
    printf("[!] Forgot Password Failed: User Not Found.\n");
    return 0;
  }

  String32 security_answer;

  io_get_string("Enter Security Answer: ", security_answer, 33);

  UserCredential *verification_result =
      verify_security_answer(state->credential_list, user_id, security_answer);

  if (!verification_result) {
    printf("[!] Forgot Password Failed: Wrong Verification Answer.\n");
    return 0;
  }

  String32 new_password;

  io_get_string("Enter New: ", new_password, 33);

  change_password(state->credential_list, verification_result, new_password);

  printf("[!] Change Password Successful: Please proceed to login.\n");

  return 1;
}
