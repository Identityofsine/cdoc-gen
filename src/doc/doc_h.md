# doc.h
### Types
```c
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
```
#### Functions
```c
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
```
