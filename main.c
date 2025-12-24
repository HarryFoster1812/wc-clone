#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define RETURNARG_LEN 5

int *processFile(FILE *f) {

  bool lastAlphanumeric = false;

  char c;
  int lineCount = 0;
  int wordCount = 0;
  int characterCount = 0;
  int byteCount = 0;

  int longestLine = 0;
  int currentLine = 0;

  c = fgetc(f);
  while (c != EOF) {
    // process character
    byteCount++;
    bool isAlphanum = isalnum(c);
    if (isAlphanum && !lastAlphanumeric) {
      wordCount++;
    } else if (c == '\n') {
      lineCount++;
    }
    lastAlphanumeric = isAlphanum;
    c = fgetc(f);
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
