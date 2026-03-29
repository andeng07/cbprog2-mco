#include "io_util.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <stdio.h>

#define TERMINAL_WIDTH 82

void print_title() {
  printf(GRN
         " .--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--."
         ".--..--..--. \n"
         "/ .. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. "
         "\\.. \\.. \\.. \\.. \\.. \\.. \\.. \\\n"
         "\\ \\/\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ "
         "`\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ \\/ /\n"
         " \\/ "
         "/`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'"
         "`--'\\/ / \n"
         " / /\\         _____             _                 _  _            _ "
         "         / /\\ \n"
         "/ /\\ \\       | ____| _ __ ___  | |__    ___    __| |(_)  ___   __| "
         "|        / /\\ \\\n"
         "\\ \\/ /       |  _|  | '_ ` _ \\ | '_ \\  / _ \\  / _` || | / _ \\ "
         "/ _` |        \\ \\/ /\n"
         " \\/ /        | |___ | | | | | || |_) || (_) || (_| || ||  __/| (_| "
         "|         \\/ / \n"
         " / /\\        |_____||_| |_| |_||_.__/  \\___/  \\__,_||_| \\___| "
         "\\__,_|         / /\\ \n"
         "/ /\\ \\                                                             "
         "         / /\\ \\\n"
         "\\ \\/ /        _____             _            _                     "
         "         \\ \\/ /\n"
         " \\/ /        | ____| _ __ ___  (_) ___  ___ (_)  ___   _ __   ___   "
         "         \\/ / \n"
         " / /\\        |  _|  | '_ ` _ \\ | |/ __|/ __|| | / _ \\ | '_ \\ / "
         "__|           / /\\ \n"
         "/ /\\ \\       | |___ | | | | | || |\\__ \\\\__ \\| || (_) || | | "
         "|\\__ \\          / /\\ \\\n"
         "\\ \\/ /       |_____||_| |_| |_||_||___/|___/|_| \\___/ |_| "
         "|_||___/          \\ \\/ /\n"
         " \\/ /                                                               "
         "         \\/ / \n"
         " / "
         "/\\.--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..-"
         "-..--./ /\\ \n"
         "/ /\\ \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. "
         "\\.. \\.. \\.. \\.. \\.. \\.. \\/\\ \\\n"
         "\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ "
         "`\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\'\\ `\' /\n"
         " `--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'"
         "`--'`--'`--'" RESET);

  printf("\n\n");
  io_print_centered("WELCOME TO THE EMBODIED EMISSION CALCULATOR", BGRN);
  io_print_centered("A Lasallian Initiative for a Greener Future", WHT);

  printf("\n");
  io_print_centered(
      "[SDG 9: Industry & Innovation]  [SDG 11: Sustainable Cities]", CYN);

  printf("\n");
  io_print_centered(
      "Inspired by the UN Sustainable Development Goals, this program", WHT);
  io_print_centered(
      "empowers the construction industry to track carbon footprints.", WHT);

  printf("\n");
  io_print_centered("TARGET: 40% Carbon Reduction by 2030", BGRN);

  printf("\n");
  io_print_centered("> Live the mission. Build the future. Animo La Salle!",
                    GRN);

  io_pause();
}

void io_print_header(const char *text) {
  printf("\n" GRN);
  for (int i = 0; i < TERMINAL_WIDTH; i++)
    printf("=");
  printf("\n" RESET);

  io_print_centered(text, BGRN);

  printf(GRN);
  for (int i = 0; i < TERMINAL_WIDTH; i++)
    printf("=");
  printf("\n" RESET);
}

void io_print_centered(const char *text, const char *color) {
  int len = (int)strlen(text);
  int padding = (TERMINAL_WIDTH - len) / 2;
  if (padding < 0)
    padding = 0;

  printf("%s%*s%s%s\n", color, padding, "", text, RESET);
}

void io_get_int(const char *prompt, int *result) {
  io_print_input(prompt);

  int success = 0;

  do {
    success = scanf("%d", result) == 1;

    if (!success)
      io_print_error("Input invalid. Please try again.");

    io_clear_buffer();
  } while (!success);
}

void io_get_uint(const char *prompt, unsigned int *result) {
  io_print_input(prompt);

  int success = 0;

  do {
    success = scanf("%u", result) == 1;

    if (!success)
      io_print_error("Input invalid. Please try again.");

    io_clear_buffer();
  } while (!success);
}

void io_get_float(const char *prompt, float *result) {
  io_print_input(prompt);

  int success = 0;

  do {
    success = scanf("%f", result) == 1;

    if (!success)
      io_print_error("Input invalid. Please try again.");

    io_clear_buffer();
  } while (!success);
}

void io_get_string(const char *prompt, char *buffer, int max_count) {
  int success = 0;

  io_print_input(prompt);

  while (!success) {
    int count = 0;
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
      if (count < max_count - 1) {
        *(buffer + count) = (char)ch;
      }
      count++;
    }

    if (count > max_count - 1) {
      *buffer = '\0';
      io_print_error("Content too long.");
    } else {
      *(buffer + count) = '\0';

      int start = 0;
      while (isspace((unsigned char)*(buffer + start)))
        start++;

      int end = count - 1;
      while (end >= start && isspace((unsigned char)*(buffer + end)))
        end--;

      int i = 0;
      while (start <= end)
        *(buffer + i++) = *(buffer + start++);

      *(buffer + i) = '\0';

      success = 1;
    }
  }
}

void io_clear_buffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void io_pause() {
  printf("\n");
  io_print_info("Press [ENTER] to continue...");
  getchar();
}

void io_print_info(const char *message) {
  printf(CYN "\n[Info] " RESET WHT "%s\n" RESET, message);
}

void io_print_success(const char *message) {
  printf(BGRN "\n[Success] " RESET WHT "%s\n" RESET, message);
}

void io_print_warning(const char *message) {
  printf(YEL "\n[Warning] " RESET WHT "%s\n" RESET, message);
}

void io_print_error(const char *message) {
  printf(BRED "\n[Error] " RESET WHT "%s\n" RESET, message);
}

void io_print_input(const char *message) {
  printf(CYN "[Input] " RESET WHT "%s: " RESET, message);
}
