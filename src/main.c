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
	printf("  -i, --ignore\t\t\tIgnore directories\n");
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
	
	
	//char *path = "/home/kevin/software/xlan/";
	
	char *root = NULL;
	
	if (argc > 1) {
	 root = argv[1];
	}
	if (root == NULL) {
		fprintf(stderr, "Error: Path Missing\n");
		print_help();
		return 1;

		//strip '/' from the end of the path if it exists
		if (root[strlen(root) - 1] == '/') {
			root[strlen(root) - 1] = '\0';
		}
	}

	List *files = search_path(root, true);
	if (files == NULL) {
		fprintf(stderr, "Error: Could not search path %s\n", root);
		return 1;
	}

	List *successful_paths = list_new();

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
			continue;
		}
		char **lines = file_contents(file);
		if (lines == NULL) {
			fprintf(stderr, "Error: Could not read file contents\n");
			return 1;
		}
		file_close(file);

		uint lines_count = content_length(lines);
		Block **block = doc_parse(lines, lines_count);
		if (block == NULL) {
			fprintf(stderr, "Error: Could not parse the file: %s\n", path);
			continue;
		}
		const char *filename = remove_ext(strip_path(path));
		const char *pathname = strip_file(path);
		const char *md_filename = CONCAT_STRING(filename, ".md");
		bool success = write_markdown(path, md_filename, CONCAT_STRING(CONCAT_STRING(pathname, "/"), md_filename), block, doc_length(block));
		if (block == NULL) {
			fprintf(stderr, "Error: Could not parse the file: %s\n", path);
			continue;
		} else {
			for (size_t i = 0; i < doc_length(block); i++) {
				free_block(block[i]);
			}
		}

		//if we got here, we can do other things and assume that a markdown file was created
		//clean double slashes from the path
		if (success)
			list_push(successful_paths, STRIP_DOUBLE_SLASHES(path));
	}
	
	Node *node = successful_paths->head;
	while(node != NULL) {
		//assume the path is a string
		char *path = (char*)node->data;
		if (path == NULL) {
			fprintf(stderr, "Error: Could not get file path\n");
			return 1;
		}
		node = node->next;
	}

	write_markdown_readme(successful_paths, root);
	

		
  return 0;
}
