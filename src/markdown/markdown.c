#include "markdown.h"
#include "../util/util.h"
#include "../file/file.h"
#include <stdio.h>

//@BLOCK
//@TITLE Write a markdown file
//@DESC Writes a markdown file from a list of blocks.
bool write_markdown(const char* source_file, const char* filename, const char* path, Block** blocks, size_t size) {

	if (size == 0) {
		printf("WARNING: %s has no blocks to write\n", source_file);
		return false;
	}

	//strip the path from the source file
	char *source = strrchr(source_file, '/');
	if (source == NULL) {
		source = source_file;
	} else {
		source++;
	}
	//get the extension of the source file
	char *ext = strrchr(source, '.');
	ext = ext + 1;	
	char *modi = NULL;

	//are you a c or h file?
	if(strcmp(ext, "h") == 0 || strcmp(ext, "c") == 0) {
		modi = ext;
		if (strcmp(ext, "cpp") == 0 || strcmp(ext, "hpp") == 0) {
			//if you are a cpp or hpp file, change the extension to cpp
			ext = "cpp";
		} else {
			//if you are a c or h file, change the extension to c
			ext = "c";
		}
	}  

	// modify the filename if c and h files
	if (modi != NULL) {
		path = remove_ext(path);
		path = CONCAT_STRING(path, "_");
		path = CONCAT_STRING(path, modi);
		path = CONCAT_STRING(path, ".md");
	}

	//open file for writing
	FILE *file = fopen(path, "w");
	if (file == NULL) {
		//@TODO error handling
		fprintf(stderr, "Error: Could not open file %s\n", path);
		return false;
	}

	// Write the title to the file
	fprintf(file, "%s %s\n", MARKDOWN_H1, source);

	// Write the blocks to the file
	for (size_t i = 0; i < size; i++) {
		Block *block = blocks[i];

		if (block == NULL) {
			printf("Error: Block is null\n");
			continue;
		}
		// Write file description (if present)
		if (block->type == FILE_DESC_BLOCK) {
			if (block->desc != NULL) {
				for (size_t j = 0; j < block->desc->size; j++) {
					if (block->desc->lines[j] != NULL && block->desc->lines[j]->content != NULL) {
						fprintf(file, "%s\n", block->desc->lines[j]->content);
					}
				}
			}
			continue;
		}
		// Write block title
		if (block->title != NULL) {
			const char *mark = MARKDOWN_H4;
			if (block->type == SECTION_BLOCK) 
				mark = MARKDOWN_H3;

			fprintf(file, "%s %s\n", mark, block->title->content);
		}

		// Write block description
		if (block->desc != NULL) {
			for (size_t j = 0; j < block->desc->size; j++) {
				if (block->desc->lines[j] != NULL && block->desc->lines[j]->content != NULL) {
					fprintf(file, "%s\n", block->desc->lines[j]->content);
				}
			}
		}

		// Write block code
		if (block->lines != NULL) {
			fprintf(file, "%s%s\n", MARKDOWN_CODEBLOCK, ext);
			Text* lines = block->lines;	
			for (size_t j = 0; j < lines->size; j++) {
				if (lines->lines[j] != NULL && lines->lines[j]->content != NULL) {
					fprintf(file, "%s\n", lines->lines[j]->content);
				}
			}
			fprintf(file, "%s\n", MARKDOWN_CODEBLOCK);
		}
	}
	fclose(file);
	printf("Wrote markdown file: %s\n", path);
	return true;
}
//@END

//@BLOCK
//@TITLE write_markdown_links 
//@DESC Parses an array of paths into markdown links.

const char** write_markdown_links(List* paths) {
	if(paths == NULL) {
		printf("Error: No paths to write\n");
		return NULL;
	}
	Node* node = paths->head;	
	if (node == NULL) {
		printf("No paths to write\n");
		return NULL;
	}
	//allocate memory for the links...
	const char** links = (const char**)fxMemAlloc(sizeof(char*) * (paths->size + 1));
	if (links == NULL) {
		printf("Error: Could not allocate memory for links\n");
		return NULL;
	}
	size_t idx = 0;
	// start to convert paths into markdown links
	while (node != NULL) {
		if (node->data == NULL) {
			printf("Error: Could not get file path\n");
			return NULL;
		}
		char *path = (char*)node->data;
		if (path == NULL) {
			printf("Error: Could not get file path\n");
			return NULL;
		}
		//TODO: I need to create a function/logic to change the path into a relative path to the root. Such as ignoring the root path and just getting the path from the root to the file. (i.e. ./src/file.c not /home/user/project/src/file.c or .././src/file.c)

		//Assume Name to be name for now, a new struct will be created to hold the name and path.
		const char *name = "name";
		const char *link = CONCAT_STRING("[", name);
		link = CONCAT_STRING(link, "](");
		link = CONCAT_STRING(link, path);
		link = CONCAT_STRING(link, ")");
		links[idx++] = link;
		node = node->next;
	}
	links[idx] = NULL;

	return links;

}
//@ENDBLOCK


bool does_readme_exist(const char* root) {
	const char* readme = CONCAT_STRING(root, "/README.md");
	FILE* file = fopen(readme, "r");
	if (file == NULL) {
		return false;
	}
	fclose(file);
	return true;
}

//@BLOCK
//@TITLE write_markdown_readme
//@DESC Writes a markdown file for the README.md file.
void write_markdown_readme(List* paths, const char* root) {
	//this function should take in the paths and output a markdown file full of links
	const char** links = write_markdown_links(paths);
	if (links == NULL) {
		printf("Error: Could not write markdown links\n");
		return;
	}

	const char* readme = CONCAT_STRING(root, "/README.md");
	readme = STRIP_DOUBLE_SLASHES(readme);
	FILE* file = fopen(readme, "w");
	if (file == NULL) {
		printf("Error: Could not open file %s\n", readme);
		return;
	}
	fprintf(file, "%s %s\n", MARKDOWN_H1, "README");
	for (size_t i = 0; links[i] != NULL; i++) {
		fprintf(file, "%s\n", links[i]);
	}
	fclose(file);

	printf("Wrote README.md file\n");

}
//@ENDBLOCK



