#include "doc/doc.h"
#include "markdown/markdown.h"
#include "file/file.h"
#include "util/util.h"
#include <stdio.h>

void print_help() {
	printf("Usage: ./docgen [options] [path]\n");
	printf("Generate markdown documentation from any source code base.\n");
	printf("If no path is provided, the current directory will be used.\n");
	printf("Options:\n");
	printf("  -h, --help\t\t\tPrint this help message and exit\n");
	printf("  -v, --version\t\t\tPrint version information and exit\n");
	printf("  -d, --debug\t\t\tPrint debug information\n");
}

void print_version() {
	printf("docgen 0.1.0\n");
}

void handle_args(int argc, char **argv) {
	if (argc == 1) {
		print_help();
		return;
	}
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			print_help();
			return;
		}
		if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
			print_version();
			return;
		}
		if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) {
			fxMemAllocMessages = true;
			fprintf(stdout, "Debug mode enabled\n");
		}
		else {
		}
	}
}


int main(int argc, char **argv) {
	
	
	char *path = "../src";
	if (argc > 1) {
		path = argv[1];
	}
	List *files = search_path(path, true);
	if (files == NULL) {
		fprintf(stderr, "Error: Could not search path %s\n", path);
		return 1;
	}

	for(int i = 0; i < files->size; i++) {
		char* path = (char*)list_get(files, i)->data;
		if (path == NULL) {
			fprintf(stderr, "Error: Could not get file path\n");
			return 1;
		}
		//printf("Parsing file: %s\n", path);
		FILE *file = file_open(path);
		if (file == NULL) {
			fprintf(stderr, "Error: Could not open file %s\n", path);
			return 1;
		}
		char **lines = file_contents(file);
		if (lines == NULL) {
			fprintf(stderr, "Error: Could not read file contents\n");
			return 1;
		}
		file_close(file);

		uint lines_count = content_length(lines);
		Block **block = doc_parse(lines, lines_count);
		write_markdown(path, CONCAT_STRING(remove_ext(strip_path(path)), ".md"), strip_file(path), block, doc_length(block));
		if (block == NULL) {
			fprintf(stderr, "Error: Could not parse the file: %s\n", path);
			continue;
		} else {
			Block *b = block[0];
			for(int i = 0; block[i] != NULL; i++ ) {
				if (block[i] == NULL) {
					printf("Block is null\n");
					break;
				}
				block[i] = NULL;
			}
		}
	}
	
		
	/*
  FILE *file = file_open("../src/file/file.h");
  char **lines = file_contents(file);
  uint lines_count = content_length(lines);
  file_close(file);
  Block **block = doc_parse(lines, lines_count);
  if (block == NULL) {
    fprintf(stderr, "Error: Could not parse the file\n");
    return 1;
  }
 	write_markdown("../src/file/file.h", "file.md", "../src/file/file.md", block, doc_length(block)); 
	*/
  return 0;
}
