  #include "encrypt.h"
  #include <string.h>
  
  int encrypt(char dest[], const char source[]) {
    int result = 0;
  
    if (is_encryptable(source)) {
      int key_size = (int)strlen(KEY);
  
      int i;
  
      for (i = 0; source[i] != '\0'; i++) {
        int key_index = i % key_size;
        int c = source[i] - RANGE_MIN;
        int k = KEY[key_index];
        dest[i] = RANGE_MIN + (c + k) % RANGE_SIZE;
      }
  
      dest[i] = '\0';
      result = 1;
    }
  
    return result;
  }
  
  int decrypt(char dest[], const char source[]) {
    int result = 0;
  
    if (is_encryptable(source)) {
  
      int key_size = (int)strlen(KEY);
  
      int i;
      for (i = 0; source[i] != '\0'; i++) {
        int key_index = i % key_size;
        int c = source[i] - RANGE_MIN;
        int k = KEY[key_index];
        dest[i] = RANGE_MIN + (c - k + RANGE_SIZE) % RANGE_SIZE;
      }
  
      dest[i] = '\0';
  
      result = 1;
    }
  
    return result;
  }
  
  int is_equal(const char encrypted[], const char raw[]) {
    int result = 0;
  
    size_t len = strlen(raw);
    char buffer[len + 1];
  
    int encryption_result = encrypt(buffer, raw);
  
    if (encryption_result && strcmp(encrypted, buffer) == 0)
      result = 1;
  
    return result;
  }
  
  int is_encryptable(const char s[]) {
    int encryptable = 1;
  
    for (int i = 0; s[i] != '\0'; i++)
      if (s[i] < RANGE_MIN || s[i] > RANGE_MAX)
        encryptable = 0;
  
    return encryptable;
  }
