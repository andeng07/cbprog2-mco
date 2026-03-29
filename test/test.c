#include "file_util.h"
#include "user.h"
#include <stdio.h>
#include <string.h>

// Helper to print test results
void assert_test(int condition, const char *test_name) {
  if (condition) {
    printf("[PASS] %s\n", test_name);
  } else {
    printf("[FAIL] %s\n", test_name);
  }
}

// --- Test Modules ---

void test_registration_flow(UserList *users, UserCredentialList *creds) {
  printf("\n--- Testing Registration ---\n");

  User newUser = {.permission = 1, .name = {"Alice", "B", "Tester"}};
  UserCredential newCred = {.recovery_answer = "Green"};
  strcpy(newCred.password, "password123");

  int success = register_user(creds, users, &newCred, &newUser);

  assert_test(success == 1, "User registration function returned success");
  assert_test(newUser.id == 1 && creds->size == 1,
              "ID synchronization correct");
}

void test_login_logic(UserList *users, UserCredentialList *creds) {
  printf("\n--- Testing Login ---\n");

  // Case: Correct Login
  // Note: Ensure login_user encrypts the input before comparing!
  User *res1 = login_user(creds, users, 1, "password123");
  assert_test(res1 != NULL && res1->id == 1, "Login with correct credentials");

  // Case: Wrong Password
  User *res2 = login_user(creds, users, 1, "wrong_pass");
  assert_test(res2 == NULL, "Login denied for wrong password");

  // Case: Non-existent ID
  User *res3 = login_user(creds, users, 99, "password123");
  assert_test(res3 == NULL, "Login denied for non-existent ID");
}

void test_recovery_and_change(UserList *users, UserCredentialList *creds) {
  printf("\n--- Testing Recovery & Password Change ---\n");

  // Verify security answer
  UserCredential *cred = verify_security_answer(creds, 1, "Green");
  assert_test(cred != NULL, "Security answer verification");

  // Change password
  int changed = change_password(creds, cred, "new_secret_777");
  assert_test(changed == 1, "Password change function returned success");

  // Verify login with new password
  User *res = login_user(creds, users, 1, "new_secret_777");
  assert_test(res != NULL, "Login successful after password change");
}

void test_permissions(UserList *users) {
  printf("\n--- Testing Permissions ---\n");

  User *alice = get_user_by_id(users, 1);

  // Test bitmask (assuming bit 1 is 'Read' and bit 2 is 'Write')
  assert_test(has_permission(alice, 1) == 1,
              "User has 'Read' permission (bit 1)");
  assert_test(has_permission(alice, 2) == 0,
              "User does NOT have 'Write' permission (bit 2)");
  assert_test(has_permission(NULL, 1) == 0, "NULL user safety check");
}

// Helper function to write a single User to a file
void write_user_elem(FILE *fp, User *u) {
  fprintf(fp, "%d %d %s %s %s\n", u->id, u->permission, u->name.first_name,
          u->name.middle_name, u->name.last_name);
}

int read_user_elem(FILE *fp, User *u) {
  return fscanf(fp, "%d %d %s %s %s", &u->id, &u->permission,
                u->name.first_name, u->name.middle_name,
                u->name.last_name) == 5;
}

void test_file_persistence() {
  printf("\n--- Testing File Persistence ---\n");

  const char *test_filename = "test_users.txt";
  UserList original_list = EMPTY_LIST;
  UserList loaded_list = EMPTY_LIST;

  User u1 = {.id = 10, .permission = 7, .name = {"John", "D", "Doe"}};
  User u2 = {.id = 20, .permission = 1, .name = {"Jane", "S", "Smith"}};
  list_push(&original_list, u1);
  list_push(&original_list, u2);

  write_list_to_file(test_filename, &original_list, write_user_elem);

  // Check if file exists (basic check)
  FILE *fp = fopen(test_filename, "r");
  assert_test(fp != NULL, "File created successfully");
  if (fp)
    fclose(fp);

  // 3. Test Loading
  load_list_from_file(test_filename, &loaded_list, read_user_elem);

  // 4. Verification
  assert_test(loaded_list.size == original_list.size,
              "Loaded list size matches");

  if (loaded_list.size >= 2) {
    assert_test(loaded_list.items[0].id == 10, "First element ID matches");
    assert_test(strcmp(loaded_list.items[1].name.first_name, "Jane") == 0,
                "Second element name matches");
  }

  // Cleanup
  // remove(test_filename); // Delete test file
  list_free(&original_list);
  list_free(&loaded_list);
}

// --- Main Test Runner ---

void run_all_tests() {
  UserList users = EMPTY_LIST;
  UserCredentialList creds = EMPTY_LIST;

  test_registration_flow(&users, &creds);
  test_login_logic(&users, &creds);
  test_recovery_and_change(&users, &creds);
  test_permissions(&users);

  test_file_persistence();

  printf("\n--- Suite Finished ---\n");

  list_free(&users);
  list_free(&creds);
}

int main() {
  run_all_tests();
  return 0;
}
