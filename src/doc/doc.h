#pragma once
#include "../delim.h"
#include "../list/list.h"
#include <stdbool.h>
#include <string.h>

//@SECTION
//@TITLE Documentation Macros

#ifndef COMMENT_DELIM
#define COMMENT_DELIM "//"
#endif

#ifndef START_LINE
#define START_LINE '@'
#endif

#ifndef END_LINE
#define END_LINE DELIM
#endif

#ifndef FILE_DESC_DELIM
#define FILE_DESC_DELIM "FILEDESC"
#define FILE_DESC_LENGTH 8
#endif

#ifndef SECTION_START_DELIM
#define SECTION_START_DELIM "SECTION"
#define SECTION_START_LENGTH 7
#endif

#ifndef SECTION_END_DELIM
#define SECTION_END_DELIM "ENDSECTION"
#define SECTION_END_LENGTH 10
#endif

#ifndef TITLE_DELIM
#define TITLE_DELIM "TITLE"
#define TITLE_LENGTH 5
#endif

#ifndef DESC_DELIM
#define DESC_DELIM "DESC"
#define DESC_LENGTH 4
#endif

#ifndef BLOCK_DELIM
#define BLOCK_DELIM "BLOCK"
#endif

#ifndef ENDBLOCK_DELIM
#define ENDBLOCK_DELIM "END"
#endif

#ifndef TODO_DELIM
#define TODO_DELIM "TODO"
#endif

#define CODE_BLOCK 0x01
#define TEXT_BLOCK 0x02
#define TODO_BLOCK 0x04
#define WARN_BLOCK 0x08
#define SECTION_BLOCK 0x10
#define FILE_DESC_BLOCK 0x20
//@END

//@SECTION
//@TITLE Concatenation Macros 
#ifndef CONCAT_DELIM

static inline const char *concat_delim(const char *delim,
                                       const char *expression) {
  char *result = malloc(strnlen(delim, 10) + strnlen(expression, 10) + 1);
  strcpy(result, delim);
  strcat(result, expression);
  return result;
}

#define CONCAT_DELIM(a) concat_delim(DELIM_STRING(START_LINE), a)

static inline bool compare_delim(const char *s, const char *expression) {
  const char *exp = CONCAT_DELIM(expression);
  return strncmp(s, exp, strnlen(exp, 10)) == 0;
}

#define COMPARE_DELIM(s, a) compare_delim(s, a)

#endif
//@END

//@SECTION
//@TITLE Types
//@DESC Types for working with documentation. These types are subject to change, I plan on adding more features to the documentation system such as having nested blocks in sections, which allow for more complex documentation structures. Eventually, I would like to have a look-up table for documentation, which would allow for a central location to access documentation for a project.
typedef struct {
  char *content;
  size_t size;
} Line;

typedef struct {
  Line **lines; // array of lines
  size_t size;  // number of lines
} Text;

typedef struct {
  Line *title;
  Text *desc;
  Text *lines;
	unsigned int type;
} Block;

typedef struct {
	Block **blocks;
	size_t size;
} Section;

typedef struct {
	Section **sections;
	size_t size;
} Doc;

//@END

//@BLOCK
//@TITLE Functions

Line *line_new(char *content, size_t size);
Text *text_new();
Text *text_push(Text *text, Line *line);
Block *block_new();

void block_set_title(Block *block, Line *title);
void block_set_desc(Block *block, Text *desc);
void block_push_line(Block *block, Line *line);

void block_print(Block *block);
void free_block(Block *block);

Block **doc_parse(char **lines, size_t size);

unsigned long doc_length(Block **block);


//@END
