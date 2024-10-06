# doc.md
## Types{
```c
typedef struct {
  char *content;
  size_t size;
} Line;
typedef struct {
  Line **lines; // array of lines
  size_t size;  // number of linesÃ™V
} Text;
typedef struct {
  Line *title;
  Text *desc;{
  Text **lines;
} Block; «ÞB{
typedef struct {
	Block **blocks;
	size_t size;{
} Section;ÞB{
typedef struct {
	Section **sections;
	size_t size;{
} Doc;
```
## Functions«ÞB{
```c
```
