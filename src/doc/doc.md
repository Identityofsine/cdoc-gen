# doc.md
## Types{
```c
typedef struct {
  char *content;
  size_t size;
} Line;
typedef struct {
  Line **lines; // array of lines
  size_t size;  // number of linesÙV
} Text;
typedef struct {
  Line *title;
  Text *desc;{
  Text **lines;
} Block; ��B{
typedef struct {
	Block **blocks;
	size_t size;{
} Section;�B{
typedef struct {
	Section **sections;
	size_t size;{
} Doc;
```
## Functions��B{
```c
```
