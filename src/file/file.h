#include "../delim.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define DELIM 0x0d
#else
#define DELIM 0x0a
#endif

char **push(char **arr, char *val) {
  size_t len = 0;
  while (arr[len] != NULL) {
    len++;
  }
  arr = realloc(arr, sizeof(char *) * (len + 2));
  arr[len] = val;
  arr[len + 1] = NULL;
  return arr;
}

//@BLOCK
//@TITLE Open a file
//@DESC Opens a file at a given path.
FILE *file_open(const char *path) {
  //@1 use fopen to open the file
  FILE *file = fopen(path, "r");
  //@n check if the file was opened successfully
  if (file == NULL) {
    fprintf(stderr, "Error: Could not open file %s\n", path);
  }
  //@ENDn
  //@1 return the file
  return file;
}
//@END

//@BLOCK
//@DESC Closes a file at a given path.
bool file_close(FILE *file) {
  //@1 use fclose to close the file
  return fclose(file) == 0;
}
//@END

//@BLOCK
//@TITLE Read the contents of a file
//@DESC Reads the contents of a file and returns them as an array of strings.
char **file_contents(FILE *file) {
  //@1 get the size of the file
  fseek(file, 0, SEEK_END);
  if (ftell(file) == -1) {
    //@TODO handle error
    fprintf(stderr, "Error: Could not read file size\n");
    return NULL;
  }
  size_t size = ftell(file);
  rewind(file);
  //@1 allocate memory for the file contents
  char *contents = malloc(size);
  if (contents == NULL) {
    //@TODO handle error
    fprintf(stderr, "Error: Could not allocate memory for file contents\n");
    return NULL;
  }
  //@1 read the file contents
  if (fread(contents, 1, size, file) != size) {
    //@TODO handle error
    fprintf(stderr, "Error: Could not read file contents\n");
    return NULL;
  } else {
    //@1 split the file contents into newlines
    size_t line_count = 0;
    char *line = strtok(contents, GET_DELIM);
    char **lines = malloc(sizeof(char *) * (line_count + 1));
    while (line != NULL) {
      lines = push(lines, line);
      line = strtok(NULL, GET_DELIM);
      line_count++;
    }
    if (lines == NULL) {
      //@TODO handle error
      fprintf(stderr, "Error: Could not allocate memory for file lines\n");
      return NULL;
    }
    lines[line_count] = "\0\0";
    return lines;
  }
  return NULL;
}
//@END

unsigned int content_length(char **lines) {
  unsigned int length = 0;
  while (strncmp(lines[length], "\0\0", 2) != 0) {
    length++;
  }
  return length;
}
