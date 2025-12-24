#include <bits/types/mbstate_t.h>
#include <limits.h>
#include <locale.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>

#define RETURNARG_LEN 5

bool defaultFlags[] = {true, true, false, true, false};

int *processFile(FILE *f) {

  bool inWord = false;

  int rawc;
  int lineCount = 0;
  int wordCount = 0;
  int characterCount = 0;
  int byteCount = 0;

  mbstate_t state;
  memset(&state, 0, sizeof(state));

  int longestLine = 0;
  int currentLine = 0;

  unsigned char buffer[MB_LEN_MAX];
  size_t bytes_in_buffer = 0;
  wchar_t current_char;

  while ((rawc = fgetc(f)) != EOF) {
    // process character

    byteCount++;
    currentLine++;

    buffer[bytes_in_buffer++] = (unsigned char)rawc;

    size_t status = mbrtowc(&current_char, buffer, bytes_in_buffer, &state);

    if (status == (size_t)-2) {
      // incomplete, read more bytes
      continue;
    } else if (status == (size_t)-1) {
      // invalid: skip first byte, reset state
      memmove(buffer, buffer + 1, --bytes_in_buffer); // shift remaining bytes
      memset(&state, 0, sizeof(state));
      continue;
    }

    // Success (status >= 0)
    bytes_in_buffer = 0;
    characterCount++;

    bool isSpace = iswspace(current_char);

    if (!isSpace && !inWord) {
      inWord = true;
      wordCount++;
    } else if (isSpace) {
      inWord = false;
    }
    if (current_char == '\n') {
      if (currentLine > longestLine)
        longestLine = currentLine;
      currentLine = 0;
      inWord = false;
      lineCount++;
    }
  }

  if (currentLine > longestLine)
    longestLine = currentLine;

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
}

void parseSingleFlag(char *flag, bool *inputFlags) {
  if (strcmp(flag, "--bytes") == 0) {
    inputFlags[3] = true;
    return;
  } else if (strcmp(flag, "--chars") == 0) {
    inputFlags[2] = true;
    return;
  } else if (strcmp(flag, "--words") == 0) {
    inputFlags[1] = true;
    return;
  } else if (strcmp(flag, "--lines") == 0) {
    inputFlags[0] = true;
    return;
  } else if (strcmp(flag, "--max-line-length") == 0) {
    inputFlags[4] = true;
    return;
  } else if (strcmp(flag, "--help") == 0) {
    // write help info
    exit(EXIT_SUCCESS);
  } else if (strcmp(flag, "--version") == 0) {
    // write version info
    exit(EXIT_SUCCESS);
  }
}

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  bool *printFlags = (bool *)&defaultFlags;
  bool flagSet = false;
  bool *inputFlags = calloc(RETURNARG_LEN, sizeof(bool));

  int inputFileCount = 0;
  int *inputFiles = malloc(sizeof(int) * argc);
  int *outputTotals = calloc(RETURNARG_LEN, sizeof(int));

  for (int i = 1; i < argc; ++i) {
    if (strncmp(argv[i], "--", 2) == 0) {
      flagSet = true;
      printFlags = inputFlags;
      parseSingleFlag(argv[i], inputFlags);
    } else if (argv[i][0] == '-') {
      // it is a char flag
      flagSet = true;
      printFlags = inputFlags;
      for (int j = 1; j < strlen(argv[i]); ++j) {
        switch (argv[i][j]) {
        case 'l':
          inputFlags[0] = true;
          break;
        case 'w':
          inputFlags[1] = true;
          break;
        case 'm':
          inputFlags[2] = true;
          break;
        case 'c':
          inputFlags[3] = true;
          break;
        case 'L':
          inputFlags[4] = true;
          break;
        default:
          printf("INVALID FLAG %c please check --help for more information",
                 argv[i][j]);
        }
      }
    } else {
      // we can interpret it as an input file
      inputFiles[inputFileCount++] = i;
    }
  }

  if (!flagSet) {
    free(inputFlags);
  }
  if (!inputFileCount) {
    free(inputFiles);

    // take file from stdin
    int *statistics = processFile(stdin);
    printStats(statistics, (bool *)printFlags);
    printf("\n");
    return EXIT_SUCCESS;
  }

  // else we take the file list

  for (int i = 0; i < inputFileCount; ++i) {
    int fileIndex = inputFiles[i];
    FILE *f = fopen(argv[fileIndex], "r");
    if (!f) {
      printf("Failed to find file %s\n", argv[fileIndex]);
    } else {
      int *statistics = processFile(f);
      fclose(f);
      if (!statistics) {
        // something bad happened in processFile
        printf("Process file failed to malloc return struct\n");
        exit(EXIT_FAILURE);
      }
      printStats(statistics, printFlags);
      printf(" %s\n", argv[fileIndex]);

      // aggregate total
      outputTotals[0] += statistics[0];
      outputTotals[1] += statistics[1];
      outputTotals[2] += statistics[2];
      outputTotals[3] += statistics[3];
      // set max line length
      if (statistics[4] > outputTotals[4]) {
        outputTotals[4] = statistics[4];
      }
      free(statistics);
    }
  }
  if (inputFileCount > 1) {
    // print total
    printStats(outputTotals, printFlags);
    printf(" total\n");
  }

  return EXIT_SUCCESS;
}
