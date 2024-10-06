# doc.c
#### Line, Text, and Block Pointer based functions 
Functions for working with Line, Text, and Block pointers. These either create, modify, and destory these pointers.
```c
Line *line_new(char *content, size_t size) {
  Line *line = fxMemAlloc(sizeof(Line));
	line->size = (size_t)fxMemAlloc(sizeof(size_t));
  line->content = fxMemAlloc(size + 1);
	strncpy(line->content, content, size + 1);
	line->size = size;
  return line;
}
Text *text_new() {
  Text *text = fxMemAlloc(sizeof(Text));
	text->lines = fxMemAlloc(sizeof(Line *));
  text->size = 0;
  return text;
}
Text *text_push(Text *text, Line *line) {
	if (text == NULL) {
		printf("Error: Text is null\n");
		return NULL;
	}
	int sz = (text->size);
  text->size++;
	text->lines = fxMemRealloc(text->lines, sizeof(Line *) * text->size);  // Resize for Line* pointers
	text->lines[sz] = line;
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
  block->lines = text_new();
	block->type = 0;
  return block;
}
void block_set_title(Block *block, Line *title) { 
	block->title = title; 
}
void block_set_desc(Block *block, Text *desc) { block->desc = desc; }
void block_push_line(Block *block, Line *line) {
  text_push(block->lines, line);
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
      for (size_t i = 0; i < (block->lines)->size; i++) {
        printf("  %s\n", (block->lines)->lines[i]->content);
      }
    }
  }
}
```
#### Free Functions
Functions for freeing memory allocated for Line, Text, and Block pointers.
```c
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
		fxMemFree(block->lines);
	}
	fxMemFree(block);
}
```
#### doc_parse 
Parses a file into a list of blocks. The function reads the file line by line and creates a block for each section. The function returns an array of blocks.
```c
Block **doc_parse(char **lines, size_t size) {
	//create linked list
  List *blocks = list_new();
  // create a new block
	//just defining the block and associated variables
  Block *block = block_new();
  block_set_title(block, line_new("Untitled", 8));
  int block_type = 0;
  bool in_block = false;
	//null safe
	if(size == 0) {
		printf("Error: File Empty\n");
		return NULL;
	}
	//null safe
	if(lines == NULL) {
		printf("Error: Lines is null\n");
		return NULL;
	}
	//iterate through the lines (n^2)
  for (int i = 0; i < size; i++) {
    bool ignore = false;
    char *line = lines[i];
		//null safe
		if (line == NULL) {
			printf("Error: Line is null\n");
			continue;
		}
		//if the line, is somehow, longer then 25000 characters, then it is too long...
    size_t strlen = strnlen(line, 25000);
    if (strlen == 25000) {
      printf("Fatal error: line too long\n");
      return NULL;
    }
		//handle empty lines
    if (strlen == 0) {
      continue;
    }
    // now check if the line is a comment
		//strip whitespace from the start of the line
		const char* strip_line = strip_whitespace_from_start(line);
    if (strncmp(strip_line, COMMENT_DELIM, 2) == 0) {
      // read the comment
      for (int j = 2; j < strlen; j++) {
				//skip whitespace comments
        if (strip_line[j] == ' ') {
          continue;
        }
				//check if the comment is a expected comment
        if (strip_line[j] == START_LINE) {
          // check if the comment is a section
          const char *section = strip_line + j;
					if (COMPARE_DELIM(section, SECTION_START_DELIM)) {
						if (in_block) {
							//TODO handle error
							printf("Error: SECTION not closed (Line: %d)\n", i);
							continue;
						}
						in_block = true;
						block_type = SECTION_BLOCK; 
						//start section
					}
					else if (COMPARE_DELIM(section, BLOCK_DELIM)) {
						if (in_block) {
							//TODO handle error
							printf("Error: Block not closed (Line: %d)\n", i);
							continue;
						}
            in_block = true;
						//start block
          } else if (COMPARE_DELIM(section, ENDBLOCK_DELIM)) {
						if(!in_block) {
							//TODO handle error
							printf("Error: Block not opened (Line:%d)\n", i);
							continue;
						}
						//handle the end block
						goto push_block;
          } else if (COMPARE_DELIM(section, TITLE_DELIM)) {
						if (!in_block) {
							//TODO handle Error
							printf("Error: Title not in block (Line:%d)\n", i);
							continue;
						}
						//set title block
            const char *title = strip_line + j + TITLE_LENGTH + 2;
            block_set_title(block, line_new(title, strlen - j - TITLE_LENGTH - 2));
          } else if (COMPARE_DELIM(section, DESC_DELIM)) {
						//insert description block
						if (!in_block) {
							//TODO handle Error
							printf("Error: Description not in block (Line:%d)\n", i);
							continue;
						}
						//make new text block
						Text *desc = text_new();
						//make new line block and allocate memory for the line
						Line *line = line_new(strip_line + j + DESC_LENGTH + 2, strlen - j - DESC_LENGTH - 2);
						//push line to text block
						text_push(desc, line);
						//set description block
						block_set_desc(block, desc);
						block_type = 1;
					} else if (COMPARE_DELIM(section, FILE_DESC_DELIM)) {
						//insert file description block (special case)
						Text *desc = text_new();
						Line *line = line_new(strip_line + j + FILE_DESC_LENGTH + 2, strlen - j - FILE_DESC_LENGTH - 2);
						text_push(desc, line);
						block_set_desc(block, desc);
						block_type = FILE_DESC_BLOCK;
						goto push_block;
					}
          ignore = true;
					
        }
      }
    }
    if (in_block && !ignore) {
			//push line to block (for now this is soley for code)
      block_push_line(block, line_new(line, strlen));
    }
		continue;
		//goto marker to quickly push block and allocate a new one
		push_block: {
			block->type = block_type;
			list_push(blocks, block);
			block = block_new();
			block_type = 0;
			block_set_title(block, line_new("Untitled", 9));
			in_block = false;
		}
  }
	//use linked list to create array
  return (Block **)list_to_array(blocks, sizeof(Block));
}
```
