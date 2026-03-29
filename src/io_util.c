#include "io_util.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Helper for consistent dividers
void print_line(int width) {
  printf("  +");
  for (int i = 0; i < width; i++)
    printf("-");
  printf("+\n");
}

void io_clear_buffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void io_print_header(const char *text) {
  printf("\n");

  const int WIDTH = 60;
  int len = (int)strlen(text);

  if (len > WIDTH - 8)
    len = WIDTH - 8;

  int total_padding = WIDTH - len - 4;
  int left_padding = total_padding / 2;
  int right_padding = total_padding - left_padding;

  printf("\n");
  for (int i = 0; i < WIDTH; i++)
    printf("=");
  printf("\n");

  printf("||%*s%s%*s||\n", left_padding, "", text, right_padding, "");

  for (int i = 0; i < WIDTH; i++)
    printf("=");
  printf("\n");
}
int io_get_int(const char *prompt, unsigned int *result) {
  printf("%s", prompt);

  if (scanf("%u", result) != 1) {
    io_clear_buffer();
    return 0;
  }

  io_clear_buffer();
  return 1;
}

void io_get_string(const char *prompt, char *buffer, int size) {
  char raw[1024];

  while (1) {
    printf("%s", prompt);

    if (fgets(raw, sizeof(raw), stdin) == NULL)
      continue;

    int start = 0;
    while (isspace((unsigned char)raw[start]))
      start++;

    if (raw[start] == '\0') {
      printf("[!] Input cannot be empty.\n");
      continue;
    }

    int end = strlen(raw) - 1;
    while (end > start && isspace((unsigned char)raw[end]))
      end--;

    int final_len = end - start + 1;

    if (final_len >= size) {
      printf("[!] Content too long. Max allowed: %d\n", size - 1);
      continue;
    }

    memcpy(buffer, &raw[start], final_len);
    buffer[final_len] = '\0';

    break;
  }
}

void io_pause() {
  printf("\nPress [ENTER] to continue...");
  getchar();
}
