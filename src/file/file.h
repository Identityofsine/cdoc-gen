#include "../delim.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <libgen.h>
#include "../list/list.h"

#ifdef _WIN32
#define DELIM 0x0d
#else
#define DELIM 0x0a
#endif


char *CONCAT_DIRECTORY(const char *path, const char *name) {
	char *new_path = malloc(strlen(path) + strlen(name) + 2);
	strcpy(new_path, path);
	strcat(new_path, "/");
	strcat(new_path, name);
	return new_path;
}

bool is_md(const char *path) {
	char *ext = strrchr(path, '.');
	if (ext == NULL) {
		return false;
	}
	return strcmp(ext, ".md") == 0;
}

char *strip_path(const char *path) {
	char *source = strrchr(path, '/');
	if (source == NULL) {
		source = path;
	} else
		source++;
	return source;
}

char* strip_file(char *path) {
	//../src/file/file.h
	//remove the file name from the path
	char *source = strdup(path);
	char *dir = dirname(source);
	return dir;
}

char *strip_ext(const char *path) {
	char *ext = strrchr(path, '.');
	if (ext == NULL) {
		return NULL;
	}
	return ext;
}

char *remove_ext(const char *path) {
	char *ext = strrchr(path, '.');
	if (ext == NULL) {
		return NULL;
	}
	char *new_path = malloc(strlen(path) - strlen(ext) + 1);
	strncpy(new_path, path, strlen(path) - strlen(ext));
	new_path[strlen(path) - strlen(ext)] = '\0';
	return new_path;
}


//@SECTION File I/O

//@BLOCK
//@TITLE Push a value onto an array
//@DESC Pushes a value onto an array of strings.
char **push(char **arr, char *val) {
  size_t len = 0;
  while (arr[len] != NULL) {
    len++;
  }
  arr = realloc(arr, sizeof(char *) * (len + 2));
  arr[len] = val;
  arr[len + 1] = NULL;
  return arr;
}
//@END

//@BLOCK
//@TITLE Open a file
//@DESC Opens a file at a given path.
FILE *file_open(const char *path) {
  //@1 use fopen to open the file
  FILE *file = fopen(path, "r");
  //@n check if the file was opened successfully
  if (file == NULL) {
    fprintf(stderr, "Error: Could not open file %s\n", path);
  }
  //@n
  //@1 return the file
  return file;
}
//@END

//@BLOCK
//@DESC Closes a file at a given path.
bool file_close(FILE *file) {
  //@1 use fclose to close the file
  return fclose(file) == 0;
}
//@END

//@ENDSECTION

//@BLOCK
//@TITLE Read the contents of a file
//@DESC Reads the contents of a file and returns them as an array of strings.
char **file_contents(FILE *file) {
  //@1 get the size of the file
  fseek(file, 0, SEEK_END); 
  if (ftell(file) == -1) {
    //@TODO handle error
    fprintf(stderr, "Error: Could not read file size\n");
    return NULL;
  }
  size_t size = ftell(file);
  rewind(file);
  //@1 allocate memory for the file contents
  char *contents = malloc(size);
  if (contents == NULL) {
    //@TODO handle error
    fprintf(stderr, "Error: Could not allocate memory for file contents\n");
    return NULL;
  }
  //@1 read the file contents
  if (fread(contents, 1, size, file) != size) {
    //@TODO handle error
    fprintf(stderr, "Error: Could not read file contents\n");
    return NULL;
  } else {
    //@1 split the file contents into newlines
    size_t line_count = 0;
    char *line = strtok(contents, GET_DELIM);
    char **lines = malloc(sizeof(char *) * (line_count + 1));
    while (line != NULL) {
      lines = push(lines, line);
      line = strtok(NULL, GET_DELIM);
      line_count++;
    }
    if (lines == NULL) {
      //@TODO handle error
      fprintf(stderr, "Error: Could not allocate memory for file lines\n");
      return NULL;
    }
    lines[line_count] = "\0\0";
    return lines;
  }
  return NULL;
}
//@END



//@BLOCK
//@TITLE search_path
//@DESC Searches for files in a given path. This function should return an array of strings containing the paths of the files found, there should be an option to search recursively and for specific file extensions 
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
		if (entry->d_type == DT_REG && !is_md(entry->d_name)) { // If the entry is a regular file && ignore .md
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

unsigned int content_length(char **lines) {
	//null safe	
	if (lines == NULL) {
		return 0;
	}
	unsigned int length = 0;
	for (unsigned int i = 0; lines[i] != NULL; i++) {
		length++;
	}
  return length;
}
