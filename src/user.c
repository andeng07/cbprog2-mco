#include "user.h"
#include "encrypt.h"
#include "file_util.h"

int read_cred_element(FILE *file_ptr, UserCredential *credential) {
  if (!file_ptr || !credential)
    return 0;

  return fscanf(file_ptr, CRED_IN_FMT, &credential->id, credential->password,
                credential->recovery_answer) == 3;
}

void write_cred_element(FILE *file_ptr, const UserCredential *credential) {
  if (!file_ptr || !credential)
    return;

  fprintf(file_ptr, CRED_OUT_FMT, credential->id, credential->password,
          credential->recovery_answer);
}

void load_creds_from_file(UserCredentialList *credentials) {
  load_list_from_file(CREDS_FILE_NAME, credentials, read_cred_element);
}

void save_creds_to_file(UserCredentialList *credentials) {
  write_list_to_file(CREDS_FILE_NAME, credentials, write_cred_element);
}

int read_user_element(FILE *file_ptr, User *user) {
  if (!file_ptr || !user)
    return 0;

  return fscanf(file_ptr, USER_IN_FMT, &user->id, &user->permission,
                user->name.first_name, user->name.middle_name,
                user->name.last_name) == 5;
}

void write_user_element(FILE *file_ptr, const User *user) {
  if (!file_ptr || !user)
    return;

  fprintf(file_ptr, USER_OUT_FMT, user->id, user->permission,
          user->name.first_name, user->name.middle_name, user->name.last_name);
}

void load_users_from_file(UserList *users) {
  load_list_from_file(USERS_FILE_NAME, users, read_user_element);
}

void save_users_to_file(UserList *users) {
  write_list_to_file(USERS_FILE_NAME, users, write_user_element);
}

int is_user_id_equal(User *a, User *b) { return a->id == b->id; }

int is_cred_id_equal(UserCredential *a, UserCredential *b) {
  return a->id == b->id;
}

UserCredential *get_cred_by_id(UserCredentialList *credentials,
                               unsigned int id) {
  UserCredential *found_cred = NULL;
  UserCredential search_cred = {.id = id};

  list_find_first(credentials, search_cred, is_cred_id_equal, &found_cred);

  return found_cred;
}

User *get_user_by_id(UserList *users, unsigned int id) {
  User *found_user = NULL;
  User search_user = {.id = id};

  list_find_first(users, search_user, is_user_id_equal, &found_user);

  return found_user;
}

int register_user(UserCredentialList *credentials, UserList *users,
                  UserCredential *credential, User *user) {
  if (!encrypt(credential->password, credential->password) ||
      !encrypt(credential->recovery_answer, credential->recovery_answer))
    return 0;

  credential->id = credentials->size + 1;
  user->id = users->size + 1;

  list_push(credentials, *credential);
  list_push(users, *user);

  save_creds_to_file(credentials);
  save_users_to_file(users);

  return 1;
}

User *login_user(UserCredentialList *credentials, UserList *users,
                 unsigned int id, String32 password) {
  UserCredential *found_cred = get_cred_by_id(credentials, id);

  if (!found_cred || !is_equal(found_cred->password, password))
    return NULL;

  User *authenticated_user = get_user_by_id(users, id);

  return authenticated_user;
}

UserCredential *verify_security_answer(UserCredentialList *credentials,
                                       unsigned int id, String32 answer) {
  UserCredential *found_cred = get_cred_by_id(credentials, id);

  if (!found_cred || !is_equal(found_cred->recovery_answer, answer))
    return NULL;

  return found_cred;
}

int change_password(UserCredentialList *credentials, UserCredential *credential,
                    String32 new_pass) {
  if (!credentials)
    return 0;

  encrypt(credential->password, new_pass);

  save_creds_to_file(credentials);

  return 1;
}

int update_credential(UserCredentialList *credentials,
                      UserCredential *credential, String32 new_pass,
                      String32 new_rec_ans) {
  if (!credentials || !credential)
    return 0;

  String32 enc_pass_tmp;
  String32 enc_rec_tmp;

  if (new_pass && !encrypt(enc_pass_tmp, new_pass))
    return 0;

  if (new_rec_ans && !encrypt(enc_rec_tmp, new_rec_ans))
    return 0;

  if (new_pass)
    strcpy(credential->password, enc_pass_tmp);
  if (new_rec_ans)
    strcpy(credential->recovery_answer, enc_rec_tmp);

  save_creds_to_file(credentials);

  return 1;
}

int update_user(UserList *users) {
  if (!users)
    return 0;

  save_users_to_file(users);
  return 1;
}

int delete_user(UserCredentialList *credentials, UserList *users,
                UserCredential *credential, User *user) {
  if (!credentials || !users || !credential || !user)
    return 0;

  int found = 0;

  for (int i = 0; i < credentials->size && !found; i++) {
    if (credentials->items[i].id == credential->id) {
      list_remove_at(credentials, i);
      found = 1;
    }
  }

  found = 0;

  for (int i = 0; i < credentials->size && !found; i++) {
    if (users->items[i].id == user->id) {
      list_remove_at(users, i);
      found = 1;
    }
  }

  save_creds_to_file(credentials);
  save_users_to_file(users);

  return 1;
}

int user_comparator(const User *a, const User *b, int mode, int direction) {
  int result = 0;

  switch (mode) {
  case 1:
    if (a->id < b->id)
      result = -1;
    else if (a->id > b->id)
      result = 1;
    break;

  case 2:
    if (a->permission < b->permission)
      result = -1;
    else if (a->permission > b->permission)
      result = 1;
    break;

  case 3:
    result = strcmp(a->name.last_name, b->name.last_name);
    if (result == 0) {
      result = strcmp(a->name.first_name, b->name.first_name);
      if (result == 0) {
        result = strcmp(a->name.middle_name, b->name.middle_name);
      }
    }
    break;

  default:
    return 0;
  }

  return result * direction;
}

int has_permission(User *user, unsigned int permission_bit) {
  if (!user)
    return 0;
  return (user->permission & permission_bit) != 0;
}
