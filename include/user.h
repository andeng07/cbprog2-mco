#ifndef USER_H
#define USER_H

#include "string_util.h"
#include "list_util.h"
#include <stdio.h>

// PERMISSIONS
#define PERM_ACCOUNT_LIST           (1 << 0)
#define PERM_ACCOUNT_REGISTER       (1 << 1)
#define PERM_ACCOUNT_EDIT_USER      (1 << 2)
#define PERM_ACCOUNT_EDIT_CRED      (1 << 3)
#define PERM_ACCOUNT_DELETE         (1 << 4)
#define PERM_MATERIAL_LIST          (1 << 5)
#define PERM_MATERIAL_ADD           (1 << 6)
#define PERM_MATERIAL_DELETE        (1 << 7)
#define PERM_PROJECT_LIST           (1 << 8)
#define PERM_PROJECT_VIEW           (1 << 9)
#define PERM_PROJECT_ADD            (1 << 10)
#define PERM_PROJECT_DELETE         (1 << 11)
#define PERM_PROJECT_SECTION_ADD    (1 << 12)
#define PERM_PROJECT_SECTION_DELETE (1 << 13)

#define USER_PERMS (PERM_MATERIAL_LIST | PERM_PROJECT_LIST | PERM_PROJECT_VIEW)
#define PROJECT_MANAGER_PERMS (USER_PERMS | PERM_MATERIAL_ADD | PERM_MATERIAL_DELETE | PERM_PROJECT_ADD | PERM_PROJECT_DELETE | PERM_PROJECT_SECTION_ADD | PERM_PROJECT_SECTION_DELETE)
#define ADMIN_PERMS (PROJECT_MANAGER_PERMS | PERM_ACCOUNT_LIST | PERM_ACCOUNT_REGISTER | PERM_ACCOUNT_EDIT_USER | PERM_ACCOUNT_EDIT_CRED | PERM_ACCOUNT_DELETE)

#define USERS_FILE_NAME "users.txt"
#define CREDS_FILE_NAME "creds.txt"

#define CRED_IN_FMT    " %u,%[^,],%[^\n]"
#define CRED_OUT_FMT   "%u,%s,%s\n"

#define USER_IN_FMT    " %u,%u,%[^,],%[^,],%[^\n]"
#define USER_OUT_FMT   "%u,%u,%s,%s,%s\n"

typedef enum {
    GUEST = 0,          // Login / Register
    USER = 1,           // View Projects
    PM = 2,             // Create Projects
    COMPANY_ADMIN = 3,  // Access Project History
    APP_ADMIN = 4       // Records from different companies / Catalogue
} Role;

typedef struct {
  String32 first_name;
  String32 middle_name;
  String32 last_name;
} Name;

typedef struct {
  unsigned int id;
  unsigned int permission;
  Name name;
} User;

typedef List(User) UserList;

typedef struct {
  unsigned int id;
  String32 password; // encrypted representation of the password
  String32 recovery_answer; // Name of the user's favorite animal
} UserCredential;

typedef List(UserCredential) UserCredentialList;

// Credential I/O operations
int read_cred_element(FILE *file_ptr, UserCredential *credential);
void write_cred_element(FILE *file_ptr, const UserCredential *credential);
void load_creds_from_file(UserCredentialList *credentials);
void save_creds_to_file(UserCredentialList *credentials);

// User I/O operations
int read_user_element(FILE *file_ptr, User *user);
void write_user_element(FILE *file_ptr, const User *user);
void load_users_from_file(UserList *users);
void save_users_to_file(UserList *users);

int is_user_id_equal(User *a, User *b);
int is_cred_id_equal(UserCredential *a, UserCredential *b);

UserCredential* get_cred_by_id(UserCredentialList *credentials, unsigned int id);
User* get_user_by_id(UserList *users, unsigned int id);

int register_user(UserCredentialList *credentials, UserList *users, UserCredential *credential, User *user);
User *login_user(UserCredentialList *credentials, UserList *users, unsigned int id, String32 password);

UserCredential *verify_security_answer(UserCredentialList *credentials, unsigned int id, String32 answer);

int change_password(UserCredentialList *credentials, UserCredential *credential, String32 new_pass);

int update_credential(UserCredentialList *credentials, UserCredential *credential, String32 new_pass, String32 new_rec_ans);
int update_user(UserList *users);

int delete_user(UserCredentialList *credentials, UserList *users, UserCredential *credential, User *user);

int user_comparator(const User *a, const User *b, int mode, int direction);

int has_permission(User *user, unsigned int permission_bit);

#endif // USER_H
