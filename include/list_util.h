#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * macro that defines a generic dynamic array structure for a given type T.
 */
#define List(T) struct { \
    T *items; \
    int size; \
    int cap; \
}

/**
 * initializer for a list structure to set its members to null and zero.
 */
#define EMPTY_LIST { NULL, 0, 0 }

/**
 * releases the heap-allocated memory of a list and resets its members.
 */
#define list_free(list) do { \
  if ((list)->items) free((list)->items); \
  (list)->items = NULL; \
  (list)->size = (list)->cap = 0; \
} while (0)

/**
 * appends an element to the list. automatically handles memory reallocation
 * if the size exceeds current capacity.
 */
#define list_push(list, element) do { \
  if ((list)->size >= (list)->cap) { \
    int new_cap = ((list)->cap == 0) ? 4 : (list)->cap * 2; \
    void *tmp = realloc((list)->items, sizeof(*(list)->items) * new_cap); \
    if (tmp) { \
      (list)->items = tmp; \
      (list)->cap = new_cap; \
    } \
  } \
  (list)->items[(list)->size++] = (element); \
} while (0)

/**
 * removes an element at a specific index and shifts subsequent elements to fill the gap.
 */
#define list_remove_at(list, idx) do { \
  if ((idx) >= 0 && (idx) < (list)->size) { \
    int move_count = (list)->size - (idx) - 1; \
    if (move_count > 0) { \
      memmove(&(list)->items[idx], &(list)->items[(idx) + 1], \
              sizeof(*(list)->items) * move_count); \
    } \
    (list)->size--; \
  } \
} while (0)

/**
 * iterates through the list and returns a pointer to the first item matching
 * the provided element according to the comparison function.
 */
#define list_find_first(list, element, compare_func, result_ptr) do { \
  *(result_ptr) = NULL; \
  for (int _i = 0; _i < (list)->size && *(result_ptr) == NULL; _i++) { \
    if (compare_func(&(list)->items[_i], &(element))) { \
      *(result_ptr) = &(list)->items[_i]; \
    } \
  } \
} while (0)

/**
 * appends all items from a source list that match a criteria into a destination list.
 */
#define list_filter(dest_list, source_list, element, compare_func) do { \
  for (int _i = 0; _i < (source_list)->size; _i++) { \
    if (compare_func(&(source_list)->items[_i], &(element))) { \
        list_push(dest_list, (source_list)->items[_i]); \
    } \
  } \
} while (0)

/**
 * performs an in-place selection sort on the list using a comparison function.
 */
#define list_sort(list, sort_cmp) do { \
  if ((list)->size >= 2) { \
    for (int _i = 0; _i < (list)->size - 1; _i++) { \
      int _min = _i; \
      for (int _j = _i + 1; _j < (list)->size; _j++) { \
        if (sort_cmp(&(list)->items[_j], &(list)->items[_min]) < 0) \
          _min = _j; \
      } \
      if (_min != _i) { \
        __typeof__((list)->items[0]) _tmp = (list)->items[_i]; \
        (list)->items[_i] = (list)->items[_min]; \
        (list)->items[_min] = _tmp; \
      } \
    } \
  } \
} while (0)

/**
 * returns current number of elements in the list.
 */
#define list_size(list) ((list)->size)

/**
 * checks if the list contains zero elements.
 */
#define list_empty(list) ((list)->size == 0)

/**
 * retrieves an item at a specific index.
 */
#define list_get(list, idx) ((list)->items[(idx)])

/**
 * returns a pointer to the first element in the list.
 */
#define list_first(list) (((list)->size > 0) ? &((list)->items[0]) : NULL)

/**
 * returns a pointer to the last element in the list.
 */
#define list_last(list) (((list)->size > 0) ? &((list)->items[(list)->size - 1]) : NULL)

/**
 * resets the list size to zero without deallocating the internal buffer.
 */
#define list_clear(list) ((list)->size = 0)

/**
 * list_foreach
 * helper macro for iterating over list items using a pointer loop.
 */
#define list_foreach(type, var, list) \
    for (type *var = (list)->items, *_end = (list)->items + (list)->size; var < _end; var++)

#endif // LIST_H
