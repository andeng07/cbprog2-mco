#ifndef STRING_UTIL
#define STRING_UTIL

typedef char String8[9];
typedef char String32[33];
typedef char String64[65];

int str_equal(const char a[], const char b[]);

int str_split(char *dest[], const char src[], const char delimeter);

void substring(char dest[], const char src[], int start_idx, int end_idx);

#endif
