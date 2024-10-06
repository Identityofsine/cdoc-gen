# doc.md
## Types
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
  Text **lines;
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
## Functions
```c
```
