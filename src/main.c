#include <bits/types/mbstate_t.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#define RETURNARG_LEN 5

int *processFile(FILE *f) {

  bool lastAlphanumeric = false;

  char rawc;
  int lineCount = 0;
  int wordCount = 0;
  int characterCount = 0;
  int byteCount = 0;

  mbstate_t state;
  memset(&state, 0, sizeof(state));

  int longestLine = 0;
  int currentLine = 0;

  char buffer[MB_LEN_MAX];
  size_t bytes_in_buffer = 0;
  wchar_t current_char;

  rawc = fgetc(f);
  while (rawc != EOF) {
    // process character

    byteCount++;

    buffer[bytes_in_buffer++] = rawc;

    int status =
        mbrtowc(&current_char, (char *)&buffer, bytes_in_buffer, &state);
    if (status < 0) {
      rawc = fgetc(f);
      continue;
    }

    // character decoding complete
    memset(&state, 0, sizeof(state));
    bytes_in_buffer = 0;
    characterCount++;

    // reset state and buffer

    bool isAlphanum = iswalnum(current_char);
    if (isAlphanum && !lastAlphanumeric) {
      wordCount++;
    } else if (current_char == '\n') {
      lineCount++;
    }

    lastAlphanumeric = isAlphanum;

    rawc = fgetc(f);
  }
  int *return_vals = malloc(sizeof(int) * RETURNARG_LEN);
  if (!return_vals) {
    return NULL;
  }
  return_vals[0] = lineCount;
  return_vals[1] = wordCount;
  return_vals[2] = characterCount;
  return_vals[3] = byteCount;
  return_vals[4] = longestLine;
  return return_vals;
}

void printStats(int *stats, bool *printBool) {
  for (int i = 0; i < RETURNARG_LEN; ++i) {
    if (printBool[i]) {
      printf("%7d", stats[i]);
    }
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  bool printFlags[] = {true, true, false, true, false};

  if (argc == 1) {
    // need to allow std input
    int *statistics = processFile(stdin);
    printStats(statistics, (bool *)&printFlags);
  } else {
    // parse input flags
  }
  return EXIT_SUCCESS;
}
