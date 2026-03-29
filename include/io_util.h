#ifndef IO_UTIL_H
#define IO_UTIL_H

/**
 * terminal color codes
 */
#define RED     "\e[0;31m"
#define BRED    "\e[1;31m"
#define GRN     "\e[0;32m"
#define BGRN    "\e[1;32m"
#define YEL     "\e[0;33m"
#define CYN     "\e[0;36m"
#define BWHT    "\x1B[1;37m"
#define WHT     "\e[0;37m"
#define RESET   "\e[0m"

/**
 * prints the application splash screen.
 */
void print_title(void);

/**
 * prints a stylized header.
 * @param text the title text to be displayed in the header.
 */
void io_print_header(const char *text);

/**
 * prints text centered relative to the terminal width.
 * @param text the content to print.
 * @param color the ansi color code to apply.
 */
void io_print_centered(const char *text, const char *color);

/**
 * prompts for and reads a signed integer from standard input.
 * @param prompt the message displayed to the user.
 * @param result pointer to store the input value.
 */
void io_get_int(const char *prompt, int *result);

/**
 * prompts for and reads an unsigned integer from standard input.
 * @param prompt the message displayed to the user.
 * @param result pointer to store the input value.
 */
void io_get_uint(const char *prompt, unsigned int *result);

/**
 * prompts for and reads a float from standard input.
 * @param prompt the message displayed to the user.
 * @param result pointer to store the input value.
 */
void io_get_float(const char *prompt, float *result);

/**
 * reads a string from standard input, handles buffer limits, and trims whitespace.
 * @param prompt the message displayed to the user.
 * @param buffer the destination character array.
 * @param size maximum number of characters to read including null terminator.
 */
void io_get_string(const char *prompt, char *buffer, int size);

/**
 * clears the standard input buffer.
 * consumes characters until a newline or end-of-file is reached.
 */
void io_clear_buffer(void);

/**
 * halts program execution until the user presses the enter key.
 */
void io_pause(void);

/**
 * prints a cyan-labeled informational message.
 * @param message the text to display.
 */
void io_print_info(const char *message);

/**
 * prints a green-labeled success message.
 * @param message the text to display.
 */
void io_print_success(const char *message);

/**
 * prints a yellow-labeled warning message.
 * @param message the text to display.
 */
void io_print_warning(const char *message);

/**
 * prints a red-labeled error message.
 * @param message the text to display.
 */
void io_print_error(const char *message);

/**
 * prints a cyan-labeled input prompt prefix.
 * @param message the text to display before the input cursor.
 */
void io_print_input(const char *message);

#endif // IO_UTIL_H
