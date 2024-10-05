#include "doc.h"
#include "../util/util.h"
#include <stdio.h>
#include <string.h>

Line *line_new(char *content, size_t size) {
  Line *line = fxMemAlloc(sizeof(Line));
  line->content = content;
  line->size = size;
  return line;
}

Text *text_new() {
  Text *text = fxMemAlloc(sizeof(Text));
  text->lines = NULL;
  text->size = 0;
  return text;
}

Text *text_push(Text *text, Line *line) {
  text->lines = realloc(text->lines, sizeof(Line *) * (text->size + 1));
  text->lines[text->size] = line;
  text->size++;
  return text;
}

Text *text_stack(Text *text, Text *other) {
  for (size_t i = 0; i < other->size; i++) {
    text_push(text, other->lines[i]);
  }
  return text;
}

Block *block_new() {
  Block *block = fxMemAlloc(sizeof(Block));
  block->title = NULL;
  block->desc = NULL;
  block->lines = fxMemAlloc(sizeof(Text *));
  *block->lines = text_new();
  return block;
}

void block_set_title(Block *block, Line *title) { block->title = title; }

void block_set_desc(Block *block, Text *desc) { block->desc = desc; }

void block_push_line(Block *block, Line *line) {
  text_push(*block->lines, line);
}

void block_print(Block *block) {
  if (block->title != NULL) {
    printf("Title: %s\n", block->title->content);
  }
  if (block->desc != NULL) {
    printf("Description:\n");
    for (size_t i = 0; i < block->desc->size; i++) {
      printf("  %s\n", block->desc->lines[i]->content);
    }
  }
  if (block->lines != NULL) {
    printf("Lines:\n");
    if (block->lines != NULL) {
      for (size_t i = 0; i < (*block->lines)->size; i++) {
        printf("  %s\n", (*block->lines)->lines[i]->content);
      }
    }
  }
}

Block **doc_parse(char **lines, size_t size) {
  List *blocks = list_new();
  // create a new blocks

  Block *block = block_new();
  Text *desc = NULL;
  block_set_title(block, line_new("Untitled", 8));
  int block_type = 0;
  bool in_block = false;
  for (int i = 0; i < size; i++) {
    bool ignore = false;
    const char *line = lines[i];
    size_t strlen = strnlen(lines[i], 25000);
    if (strlen == 25000) {
      printf("Fatal error: line too long\n");
      return NULL;
    }
    if (strlen == 0) {
      continue;
    }
    // now check if the line is a comment
    if (strncmp(line, COMMENT_DELIM, 2) == 0) {
      // read the comment
      for (int j = 2; j < strlen; j++) {
        if (line[j] == ' ') {
          continue;
        }
        if (line[j] == '@') {
          // check if the comment is a section
          const char *section = line + j;
          if (COMPARE_DELIM(section, BLOCK_DELIM)) {
            in_block = true;
          } else if (COMPARE_DELIM(section, ENDBLOCK_DELIM)) {
            in_block = false;
            list_push(blocks, block);
            block = block_new();
            block_set_title(block, line_new("Untitled", 9));
          } else if (COMPARE_DELIM(section, TITLE_DELIM)) {
            const char *title = line + j + TITLE_LENGTH + 2;
            block_set_title(block,
                            line_new(title, strlen - j - TITLE_LENGTH - 2));
          }
          ignore = true;
        }
      }
    }
    if (in_block && !ignore) {
      block_push_line(block, line_new(line, strlen));
    }
  }

  return (Block **)list_to_array(blocks, sizeof(Block));
}
