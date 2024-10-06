#ifndef FILE_H
#pragma once
#include "../delim.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <libgen.h>
#include "../list/list.h"
#include "../util/util.h"
//@FILEDESC Functions for working with files and directories.


#define FILE_H 

#ifdef _WIN32
#define DELIM 0x0d
#else
#define DELIM 0x0a
#endif


//@SECTION
//@TITLE File Functions
//@DESC Functions for working with files and directories.

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

//@END

#endif
