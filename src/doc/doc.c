#include "doc.h"
#include "../util/util.h"
#include <stdio.h>
#include <string.h>

Line *line_new(char *content, size_t size) {
  Line *line = fxMemAlloc(sizeof(Line));
  line->content = malloc(size + 1);
	strncpy(line->content, content, size);
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
  text->lines = fxMemRealloc(text->lines, sizeof(Line *) * (text->size + 1));
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

unsigned long doc_length(Block **block) {
	unsigned long count = 0;
	while (block[count] != NULL) {
		count++;
	}
	return count;
}

void free_title(Line* title) {
	if (title == NULL) {
		return;
	}
	if (title->content == NULL) {
		fxMemFree(title);
		return;
	}
	fxMemFree(title->content);
	fxMemFree(title);
}

void free_description(Text* desc) {
	if (desc == NULL) {
		return;
	}
	if (desc->lines == NULL) {
		fxMemFree(desc);
		return;
	}
	for (size_t i = 0; i < desc->size; i++) {
		fxMemFree(desc->lines[i]->content);
		fxMemFree(desc->lines[i]);
	}
	fxMemFree(desc->lines);
	fxMemFree(desc);
}

void free_text(Text* text) {
	if (text == NULL) {
		return;
	}
	if (text->lines == NULL) {
		fxMemFree(text);
		return;
	}
	for (size_t i = 0; i < text->size; i++) {
		fxMemFree(text->lines[i]->content);
		fxMemFree(text->lines[i]);
	}
	fxMemFree(text->lines);
	fxMemFree(text);
}



void free_block(Block* block) {
	if(block == NULL) {
		return;
	}
	if (block->title != NULL) {
		free_title(block->title);
	}
	if (block->desc != NULL) {
		free_description(block->desc);
	}
	if (block->lines != NULL) {
		free_text(*block->lines);
	}
	fxMemFree(block);
}

Block **doc_parse(char **lines, size_t size) {
	//create linked list
  List *blocks = list_new();
  // create a new block
  Block *block = block_new();
  block_set_title(block, line_new("Untitled", 8));
  int block_type = 0;
  bool in_block = false;
	if(size == 0) {
		printf("Error: No lines to parse\n");
		return NULL;
	}
	if(lines == NULL) {
		printf("Error: Lines is null\n");
		return NULL;
	}
  for (int i = 0; i < size; i++) {
    bool ignore = false;
    char *line = lines[i];
		if (line == NULL) {
			printf("Error: Line is null\n");
			continue;
		}
    size_t strlen = strnlen(line, 25000);
    if (strlen == 25000) {
      printf("Fatal error: line too long\n");
      return NULL;
    }
    if (strlen == 0) {
      continue;
    }
    // now check if the line is a comment

		const char* strip_line = strip_whitespace_from_start(line);
    if (strncmp(strip_line, COMMENT_DELIM, 2) == 0) {
      // read the comment
      for (int j = 2; j < strlen; j++) {
        if (strip_line[j] == ' ') {
          continue;
        }
        if (strip_line[j] == '@') {
          // check if the comment is a section
          const char *section = strip_line + j;
          if (COMPARE_DELIM(section, BLOCK_DELIM)) {
						if (in_block) {
							//TODO handle error
							printf("Error: Block not closed\n");
							return NULL;
						}
            in_block = true;
          } else if (COMPARE_DELIM(section, ENDBLOCK_DELIM)) {
            in_block = false;
            list_push(blocks, block);
            block = block_new();
            block_set_title(block, line_new("Untitled", 9));
          } else if (COMPARE_DELIM(section, TITLE_DELIM)) {
            const char *title = strip_line + j + TITLE_LENGTH + 2;
            block_set_title(block,
                            line_new(title, strlen - j - TITLE_LENGTH - 2));
          } else if (COMPARE_DELIM(section, DESC_DELIM)) {
						Text *desc = text_new();
						Line *line = line_new(strip_line + j + DESC_LENGTH + 2, strlen - j - DESC_LENGTH - 2);
						text_push(desc, line);
						block_set_desc(block, text_new());
						block_type = 1;
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
