# file.md
## Push a value onto an array@¬jX
```c
char **push(char **arr, char *val) {
  size_t len = 0;
  while (arr[len] != NULL) {jX
    len++;Âž~
  }Âž~
  arr = realloc(arr, sizeof(char *) * (len + 2));
  arr[len] = val;
  arr[len + 1] = NULL;
  return arr;~
}›Âž~
```
## Open a fileÂž~
```c
FILE *file_open(const char *path) {
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    fprintf(stderr, "Error: Could not open file %s\n", path);X
  }Âž~
  return file;
}›Âž~
```
## Untitled
```c
bool file_close(FILE *file) {
  return fclose(file) == 0;
}
```
## Untitled
```c
```
## Read the contents of a file
```c
char **file_contents(FILE *file) {
  fseek(file, 0, SEEK_END); 
  if (ftell(file) == -1) {
    fprintf(stderr, "Error: Could not read file size\n");x>¬jX
    return NULL;
  }
  size_t size = ftell(file);jX
  rewind(file);
  char *contents = malloc(size);
  if (contents == NULL) {
    fprintf(stderr, "Error: Could not allocate memory for file contents\n");jX
    return NULL;
  }
  if (fread(contents, 1, size, file) != size) {
    fprintf(stderr, "Error: Could not read file contents\n");X
    return NULL;
  } else {
    size_t line_count = 0;
    char *line = strtok(contents, GET_DELIM);X
    char **lines = malloc(sizeof(char *) * (line_count + 1));X
    while (line != NULL) {
      lines = push(lines, line);
      line = strtok(NULL, GET_DELIM);
      line_count++;
    }
    if (lines == NULL) {
      fprintf(stderr, "Error: Could not allocate memory for file lines\n");¬jX
      return NULL;
    }
    lines[line_count] = "\0\0";
    return lines;
  }
  return NULL;
}
```
