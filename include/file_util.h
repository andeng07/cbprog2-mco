#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include "list_util.h"
#include <stdio.h>

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

#endif
