#ifndef STRING_UTIL
#define STRING_UTIL

/** 
 * fixed-size character array for strings with 8/32/64 characters plus null terminator.
 */
typedef char String8[9];
typedef char String32[33];
typedef char String64[65];

/**
 * compares two strings for equality.
 * @param a first string to compare.
 * @param b second string to compare.
 * @return 1 if strings are identical, 0 otherwise.
 */
int str_equal(const char a[], const char b[]);

/**
 * splits a string into an array of substrings based on a delimiter.
 * allocates memory for each substring in the destination array.
 * @param dest array of character pointers to store the resulting substrings.
 * @param src the source string to be split.
 * @param delimiter the character used to identify split points.
 * @return the total number of substrings created.
 */
int str_split(char *dest[], const char src[], const char delimiter);

/**
 * extracts a portion of a string into a destination buffer.
 * @param dest buffer where the substring will be stored.
 * @param src source string to extract from.
 * @param start_idx the starting index (inclusive).
 * @param end_idx the ending index (exclusive).
 */
void substring(char dest[], const char src[], int start_idx, int end_idx);

#endif
