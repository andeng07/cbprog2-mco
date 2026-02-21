#ifndef ENCRYPT_H
#define ENCRYPT_H

#define KEY "KEY"

#define RANGE_MIN 33
#define RANGE_MAX 126
#define RANGE_SIZE (RANGE_MAX - RANGE_MIN + 1)

int encrypt(char dest[], const char source[]);
int decrypt(char dest[], const char source[]);

int is_equal(const char encrypted[], const char raw[]);
int is_encryptable(const char s[]);

#endif
