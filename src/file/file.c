#include "file.h"

char *CONCAT_DIRECTORY(const char *path, const char *name) {
	char *new_path = fxMemAlloc(strlen(path) + strlen(name) + 2);
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

bool check_ext(const char *path, const char *ext) {
	char *file_ext = strrchr(path, '.');
	if (file_ext == NULL) {
		return false;
	}
	return strcmp(file_ext, ext) == 0;
}

const char* programming_exts[] = {
    ".c", ".cpp", ".h", ".hpp",            // C/C++
    ".cs",                                 // C#
    ".java", ".class",                     // Java
    ".js", ".jsx",                         // JavaScript, React
    ".ts", ".tsx",                         // TypeScript, React
    ".py",                                 // Python
    ".rb",                                 // Ruby
    ".php",                                // PHP
    ".go",                                 // Go
    ".rs",                                 // Rust
    ".swift",                              // Swift
    ".kt", ".kts", ".ktm",                 // Kotlin
    ".m", ".mm",                           // Objective-C/Objective-C++
    ".r",                                  // R
    ".pl",                                 // Perl
    ".sh",                                 // Shell Script
    ".bash", ".zsh",                       // Bash, Zsh
    ".lua",                                // Lua
    ".scala",                              // Scala
    ".dart",                               // Dart
    ".jl",                                 // Julia
    ".erl", ".hrl",                        // Erlang
    ".ex", ".exs",                         // Elixir
    ".vb", ".vbs",                         // Visual Basic
    ".hs",                                 // Haskell
    ".ml", ".mli",                         // OCaml
    ".fs", ".fsi", ".fsx", ".fsscript",    // F#
    ".groovy",                             // Groovy
    ".asm",                                // Assembly
		"\0"
};

char *strip_path(const char *path) {
	char *source = strrchr(path, '/');
	if (source == NULL) {
		source = path;
	} else
		source++;
	return source;
}

char* strip_file(char *path) {
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



//@BLOCK
//@TITLE Push a value onto an array
//@DESC Pushes a value onto an array of strings.
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

//@BLOCK
//@TITLE Read the contents of a file
//@DESC Reads the contents of a file and returns them as an array of strings.
char **file_contents(FILE *file) {
    //@1 get the size of the file
    fseek(file, 0, SEEK_END);
    long size = ftell(file);  // Use long since ftell() can return -1 on error
    if (size == -1) {
        fprintf(stderr, "Error: Could not read file size\n");
        return NULL;
    }
    rewind(file);

    //@1 allocate memory for the file contents (+1 for null terminator)
    char *contents = malloc(size + 1);
    if (contents == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for file contents\n");
        return NULL;
    }

    //@1 read the file contents
    if (fread(contents, 1, size, file) != (size_t)size) {
        fprintf(stderr, "Error: Could not read file contents\n");
        free(contents);  // Free memory on error
        return NULL;
    }
    contents[size] = '\0';  // Null terminate the file contents

    //@1 split the file contents into newlines
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
//@END


bool check_if_file_valid(const char *name) {
	for (int i = 0; programming_exts[i][0] != '\0'; i++) {
		if (check_ext(name, programming_exts[i])) {
			return true;
		}
	}
	return false;
}

//@BLOCK
//@TITLE search_path
//@DESC Searches for files in a given path. This function should return an array of strings containing the paths of the files found, there should be an option to search recursively and for specific file extensions 
List* search_path(const char *path, bool recurisve) {
	List *paths = list_new();	
	DIR *folder;
	struct dirent *entry;
	int files = 0;

	printf("Searching path: %s\n", path);

	//find a file called .cdocignore and read the contents
	//make sure to ignore the filetypes and paths in the .cdocignore file 

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
//@END

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

