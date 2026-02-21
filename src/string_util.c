#include "string_util.h"

#include <stdlib.h>
#include <string.h>

int str_equal(const char a[], const char b[]) { return strcmp(a, b) == 0; }

int str_split(char *dest[], const char src[], const char delimiter) {
  const char *copy = src;

  int split_idx = 0;
  int last_delim = -1;
  int curr_idx = 0;

  while (src[curr_idx] != '\0') {
    if (src[curr_idx] == delimiter) {
      int len = curr_idx - (last_delim + 1);

      dest[split_idx] = malloc(len + 1);
      substring(dest[split_idx], copy, last_delim + 1, curr_idx);
      dest[split_idx][len] = '\0';

      split_idx++;
      last_delim = curr_idx;
    }
    curr_idx++;
  }

  int len = curr_idx - (last_delim + 1);
  dest[split_idx] = malloc(len + 1);
  substring(dest[split_idx], copy, last_delim + 1, curr_idx);
  dest[split_idx][len] = '\0';

  return split_idx + 1;
}

void substring(char dest[], const char src[], int start_idx, int end_idx) {
  int length = end_idx - start_idx;
  memcpy(dest, src + start_idx, length);
  dest[length] = '\0';
}
