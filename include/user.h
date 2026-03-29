#ifndef USER_H
#define USER_H

#include "string_util.h"
#include "list_util.h"
#include <stdio.h>

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

int has_permission(User *user, unsigned int permission_bit);

#endif // USER_H
