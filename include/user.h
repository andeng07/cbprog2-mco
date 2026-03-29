#ifndef USER_H
#define USER_H

#include "string_util.h"
#include "list_util.h"
#include <stdio.h>

/**
 * bitwise flags used to define access control for users.
 */
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

/**
 * predefined permission sets for different user roles.
 */
#define USER_PERMS (PERM_MATERIAL_LIST | PERM_PROJECT_LIST | PERM_PROJECT_VIEW)
#define PROJECT_MANAGER_PERMS (USER_PERMS | PERM_MATERIAL_ADD | PERM_MATERIAL_DELETE | PERM_PROJECT_ADD | PERM_PROJECT_DELETE | PERM_PROJECT_SECTION_ADD | PERM_PROJECT_SECTION_DELETE)
#define ADMIN_PERMS (PROJECT_MANAGER_PERMS | PERM_ACCOUNT_LIST | PERM_ACCOUNT_REGISTER | PERM_ACCOUNT_EDIT_USER | PERM_ACCOUNT_EDIT_CRED | PERM_ACCOUNT_DELETE)

#define USERS_FILE_NAME "users.txt"
#define CREDS_FILE_NAME "creds.txt"

#define CRED_IN_FMT    " %u,%[^,],%[^\n]"
#define CRED_OUT_FMT   "%u,%s,%s\n"

#define USER_IN_FMT    " %u,%u,%[^,],%[^,],%[^\n]"
#define USER_OUT_FMT   "%u,%u,%s,%s,%s\n"

/**
 * name
 * represents a user's full name components.
 */
typedef struct {
  String32 first_name;
  String32 middle_name;
  String32 last_name;
} Name;

/**
 * user
 * represents a user's profile and system permissions.
 */
typedef struct {
  unsigned int id;
  unsigned int permission;
  Name name;
} User;

/**
 * userlist
 * type definition for a list of user structures.
 */
typedef List(User) UserList;

/**
 * usercredential
 * represents sensitive login and recovery information.
 */
typedef struct {
  unsigned int id;
  String32 password; // encrypted representation of the password
  String32 recovery_answer; // Name of the user's favorite animal
} UserCredential;

/**
 * type definition for a list of usercredential structures.
 */
typedef List(UserCredential) UserCredentialList;

// credential i/o operations

/**
 * reads a single credential entry from a file.
 * @param file_ptr pointer to the source file.
 * @param credential pointer to the destination usercredential struct.
 * @return 1 if successful, 0 otherwise.
 */
int read_cred_element(FILE *file_ptr, UserCredential *credential);

/**
 * writes a single credential entry to a file.
 * @param file_ptr pointer to the destination file.
 * @param credential pointer to the source usercredential struct.
 */
void write_cred_element(FILE *file_ptr, const UserCredential *credential);

/**
 * loads all credentials from the default creds file into a list.
 * @param credentials pointer to the list to be populated.
 */
void load_creds_from_file(UserCredentialList *credentials);

/**
 * saves all credentials from a list to the default creds file.
 * @param credentials pointer to the list to be saved.
 */
void save_creds_to_file(UserCredentialList *credentials);

// user i/o operations

/**
 * reads a single user profile entry from a file.
 * @param file_ptr pointer to the source file.
 * @param user pointer to the destination user struct.
 * @return 1 if successful, 0 otherwise.
 */
int read_user_element(FILE *file_ptr, User *user);

/**
 * writes a single user profile entry to a file.
 * @param file_ptr pointer to the destination file.
 * @param user pointer to the source user struct.
 */
void write_user_element(FILE *file_ptr, const User *user);

/**
 * loads all users from the default users file into a list.
 * @param users pointer to the list to be populated.
 */
void load_users_from_file(UserList *users);

/**
 * saves all users from a list to the default users file.
 * @param users pointer to the list to be saved.
 */
void save_users_to_file(UserList *users);

/**
 * compares two user ids for equality.
 * @return 1 if ids are equal, 0 otherwise.
 */
int is_user_id_equal(User *a, User *b);

/**
 * compares two credential ids for equality.
 * @return 1 if ids are equal, 0 otherwise.
 */
int is_cred_id_equal(UserCredential *a, UserCredential *b);

/**
 * finds a credential in a list by its id.
 * @param credentials pointer to the credential list.
 * @param id the id to search for.
 * @return pointer to the found usercredential, or null if not found.
 */
UserCredential* get_cred_by_id(UserCredentialList *credentials, unsigned int id);

/**
 * finds a user in a list by their id.
 * @param users pointer to the user list.
 * @param id the id to search for.
 * @return pointer to the found user, or null if not found.
 */
User* get_user_by_id(UserList *users, unsigned int id);

/**
 * registers a new user by encrypting details and appending to the lists and files.
 * @param credentials pointer to the credential list.
 * @param users pointer to the user list.
 * @param credential the new credential data to register.
 * @param user the new user profile data to register.
 * @return 1 if registration succeeded, 0 if encryption failed.
 */
int register_user(UserCredentialList *credentials, UserList *users, UserCredential *credential, User *user);

/**
 * authenticates a user based on id and password.
 * @param credentials list of credentials.
 * @param users list of users.
 * @param id the user id.
 * @param password the plaintext password to verify.
 * @return pointer to the authenticated user, or null if authentication fails.
 */
User *login_user(UserCredentialList *credentials, UserList *users, unsigned int id, String32 password);

/**
 * verifies if the provided recovery answer matches the stored one for a given id.
 * @param credentials list of credentials.
 * @param id the user id.
 * @param answer the plaintext recovery answer.
 * @return pointer to the usercredential if verified, null otherwise.
 */
UserCredential *verify_security_answer(UserCredentialList *credentials, unsigned int id, String32 answer);

/**
 * updates the password and/or recovery answer for a credential.
 * @param credentials pointer to the list for saving.
 * @param credential the specific credential object to update.
 * @param new_pass the new plaintext password.
 * @param new_rec_ans the new plaintext recovery answer.
 * @return 1 if success, 0 if encryption or update failed.
 */
int update_credential(UserCredentialList *credentials, UserCredential *credential, String32 new_pass, String32 new_rec_ans);

/**
 * commits current user list changes to persistent storage.
 * @param users pointer to the user list.
 * @return 1 if successful, 0 otherwise.
 */
int update_user(UserList *users);

/**
 * removes a user and their credentials from the system.
 * @param credentials pointer to the credential list.
 * @param users pointer to the user list.
 * @param credential pointer to the credential to remove.
 * @param user pointer to the user to remove.
 * @return 1 if successful, 0 if invalid parameters.
 */
int delete_user(UserCredentialList *credentials, UserList *users, UserCredential *credential, User *user);

/**
 * compares two users based on a specific attribute and direction.
 * @param a first user.
 * @param b second user.
 * @param mode comparison mode: 1 (id), 2 (permission), 3 (name).
 * @param direction sort direction: 1 (ascending), -1 (descending).
 * @return integer representing sort order.
 */
int user_comparator(const User *a, const User *b, int mode, int direction);

/**
 * checks if a user has a specific permission bit set.
 * @param user pointer to the user.
 * @param permission_bit the bitmask to check.
 * @return 1 if the user has the permission, 0 otherwise.
 */
int has_permission(User *user, unsigned int permission_bit);

#endif // USER_H
