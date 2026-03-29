#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include "list_util.h"
#include <stdio.h>

/**
 * macro that serializes a dynamic list to a text file.
 * it first writes the total element count, followed by each element 
 * using a provided element-specific writing function.
 * @param filename name of the file to create or overwrite.
 * @param list pointer to the list structure to be saved.
 * @param write_elem_func function pointer used to format and write a single element.
 */
#define write_list_to_file(filename, list, write_elem_func) do { \
  if ((filename) && (list)) { \
    FILE *_fp = fopen((filename), "w"); \
    if (_fp) { \
      int _size = (list)->size; \
      fprintf(_fp, "%d\n", _size); \
      if ((list)->items) { \
        for (int _i = 0; _i < _size; _i++) { \
            write_elem_func(_fp, &(list)->items[_i]); \
        } \
      } \
      fclose(_fp); \
    } \
  } \
} while (0)

/**
 * macro that deserializes a list from a text file.
 * it reads the element count from the first line and then iteratively 
 * parses each element using a provided reading function, pushing them into the list.
 * @param filename name of the source file to read.
 * @param list pointer to the list structure to be populated.
 * @param read_elem_func function pointer used to parse a single element from the file.
 */
#define load_list_from_file(filename, list, read_elem_func) do { \
  if ((filename) && (list)) { \
    FILE *_fp = fopen((filename), "r"); \
    if (_fp) { \
      int _count = 0; \
      if (fscanf(_fp, "%d", &_count) == 1) { \
        for (int _i = 0; _i < _count; _i++) { \
          __typeof__(*(list)->items) _temp; \
          memset(&_temp, 0, sizeof(_temp)); \
          if (read_elem_func(_fp, &_temp)) { \
            list_push((list), _temp); \
          } \
        } \
      } \
      fclose(_fp); \
    } \
  } \
} while (0)

#endif // FILE_UTIL_H