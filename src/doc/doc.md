# doc.md
## Types~
```c
typedef struct {
  char *content;
  size_t size;
} Line;
typedef struct {
  Line **lines; // array of lines
  size_t size;  // number of lines?¬jX
} Text;
typedef struct {
  Line *title;
  Text *desc;~
  Text **lines;
} Block; ›Âž~
typedef struct {
	Block **blocks;
	size_t size;~
} Section;Âž~
typedef struct {
	Section **sections;
	size_t size;~
} Doc;
```
## Functions›Âž~
```c
```
