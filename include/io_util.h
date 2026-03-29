#ifndef IO_UTIL_H
#define IO_UTIL_H

void io_clear_buffer();

void io_print_header(const char *text);

int io_get_int(const char *prompt, unsigned int *result);

void io_get_string(const char *prompt, char *buffer, int size);

void io_pause();

#endif
