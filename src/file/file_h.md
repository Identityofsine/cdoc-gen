# file.h
Functions for working with files and directories.
#### File Functions
Functions for working with files and directories.
```c
char *CONCAT_DIRECTORY(const char *path, const char *name);
bool is_md(const char *path);
char *strip_path(const char *path);
char* strip_file(char *path);
char *strip_ext(const char *path);
char *remove_ext(const char *path);
char **push(char **arr, char *val, size_t len);
FILE *file_open(const char *path);
bool file_close(FILE *file);
char **file_contents(FILE *file);
List* search_path(const char *path, bool recursive);
unsigned int content_length(char **lines);
```
