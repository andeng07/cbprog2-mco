#ifndef IO_UTIL_H
#define IO_UTIL_H

#define RED     "\e[0;31m"
#define BRED    "\e[1;31m"
#define GRN     "\e[0;32m"
#define BGRN    "\e[1;32m"
#define YEL     "\e[0;33m"
#define CYN     "\e[0;36m"
#define BWHT  "\x1B[1;37m"
#define WHT     "\e[0;37m"
#define RESET   "\e[0m"

void print_title(void);
void io_print_header(const char *text);
void io_print_centered(const char *text, const char *color);

void io_get_int(const char *prompt, int *result);
void io_get_uint(const char *prompt, unsigned int *result);
void io_get_float(const char *prompt, float *result);
void io_get_string(const char *prompt, char *buffer, int size);
void io_clear_buffer(void);
void io_pause(void);

void io_print_info(const char *message);
void io_print_success(const char *message);
void io_print_warning(const char *message);
void io_print_error(const char *message);
void io_print_input(const char *message);

#endif
