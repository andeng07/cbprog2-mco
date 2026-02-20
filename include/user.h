#ifndef USER_H
#define USER_H

#include "string_util.h"

typedef struct {
  String32 first_name;
  String32 middle_name;
  String32 last_name;
} Name;

typedef struct {
  int id;
  int permission;
  Name name;
  String32 password; // encrypted representation of the password
} User;

#endif // USER_H