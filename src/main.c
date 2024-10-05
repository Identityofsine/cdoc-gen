#include "doc/doc.h"
#include "file/file.h"
#include <stdio.h>

int main() {
  FILE *file = file_open("../src/file/file.h");
  char **lines = file_contents(file);
  uint lines_count = content_length(lines);
  file_close(file);
  Block **block = doc_parse(lines, lines_count);
  if (block == NULL) {
    fprintf(stderr, "Error: Could not parse the file\n");
    return 1;
  }
  for (size_t i = 0; i < 10; i++) {
    if (block[i] != NULL) {
      //block_print(block[i]);
    } else {
      break;
    }
  }

  return 0;
}
