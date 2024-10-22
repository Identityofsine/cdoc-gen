# file.c
#### Push a value onto an array
Pushes a value onto an array of strings.
```c
char **push(char **arr, char *val, size_t len) {
 	char **new_arr = realloc(arr, sizeof(char *) * (len + 1)); 
	if (new_arr == NULL) {
		fprintf(stderr, "Error: Could not allocate memory for array\n");
		//maybe free
		return NULL;
	}
	arr = new_arr;
	arr[len] = val;
  return arr;
}
```
#### Open a file
Opens a file at a given path.
```c
FILE *file_open(const char *path) {
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    fprintf(stderr, "Error: Could not open file %s\n", path);
  }
  return file;
}
```
#### Untitled
Closes a file at a given path.
```c
bool file_close(FILE *file) {
  return fclose(file) == 0;
}
```
#### Read the contents of a file
Reads the contents of a file and returns them as an array of strings.
```c
char **file_contents(FILE *file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);  // Use long since ftell() can return -1 on error
    if (size == -1) {
        fprintf(stderr, "Error: Could not read file size\n");
        return NULL;
    }
    rewind(file);
    char *contents = malloc(size + 1);
    if (contents == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for file contents\n");
        return NULL;
    }
    if (fread(contents, 1, size, file) != (size_t)size) {
        fprintf(stderr, "Error: Could not read file contents\n");
        free(contents);  // Free memory on error
        return NULL;
    }
    contents[size] = '\0';  // Null terminate the file contents
    size_t line_count = 0;
    char *line = strtok(contents, GET_DELIM);
    char **lines = malloc(sizeof(char *));  // Allocate initial space for one line
    if (lines == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for lines\n");
        free(contents);  // Free contents if lines allocation fails
        return NULL;
    }
    while (line != NULL) {
        lines = push(lines, line, line_count);
        if (lines == NULL) {
            free(contents);  // Free contents if reallocation fails
            return NULL;
        }
        line_count++;
        line = strtok(NULL, GET_DELIM);
    }
    lines = realloc(lines, (line_count + 1) * sizeof(char *));  // Allocate space for NULL termination
    if (lines == NULL) {
        fprintf(stderr, "Error: Could not reallocate memory for final lines array\n");
        free(contents);
        return NULL;
    }
    lines[line_count] = NULL;  // Null terminate the array of lines
    return lines;  // Return the array of lines
}
```
#### search_path
Searches for files in a given path. This function should return an array of strings containing the paths of the files found, there should be an option to search recursively and for specific file extensions 
```c
List* search_path(const char *path, bool recurisve) {
	List *paths = list_new();	
	DIR *folder;
	struct dirent *entry;
	int files = 0;
	printf("Searching path: %s\n", path);
	folder = opendir(path);
	if(folder == NULL) {
		fprintf(stderr, "Error: Could not open directory %s\n", path);
		return NULL;
	}
	
	while ((entry = readdir(folder))) {
		if (entry->d_type == DT_REG && !is_md(entry->d_name) && check_if_file_valid(entry->d_name)) { // If the entry is a regular file && ignore .md
			list_push(paths, CONCAT_DIRECTORY(path,entry->d_name));
			files++;
		} else if (entry->d_type == DT_DIR && recurisve) { // If the entry is a directory
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
				List *temp = search_path(CONCAT_DIRECTORY(path, entry->d_name), recurisve);
				paths = list_merge(paths, temp);
			}
		}
	}
	closedir(folder);
	return paths;
}
```
