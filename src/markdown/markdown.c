#include "markdown.h"
#include <stdio.h>

//@BLOCK
//@TITLE Write a markdown file
//@DESC Writes a markdown file from a list of blocks.
void write_markdown(const char* source_file, const char* filename, const char* path, Block** blocks, size_t size) {

	if (size == 0) {
		return;
	}

	FILE *file = fopen(path, "w");
	if (file == NULL) {
		fprintf(stderr, "Error: Could not open file %s\n", path);
		return;
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

	if(strcmp(ext, "h") == 0) {
		ext = "c";
	} 


	fprintf(file, "%s %s\n", MARKDOWN_H1, filename);

	// Write the blocks to the file
	for (size_t i = 0; i < size; i++) {
		Block *block = blocks[i];
		if (block->title != NULL) {
			fprintf(file, "%s %s\n", MARKDOWN_H2 ,block->title->content);
		}
		if (block->desc != NULL) {
			for (size_t j = 0; j < block->desc->size; j++) {
				fprintf(file, "%s\n", block->desc->lines[j]->content);
			}
		}
		if (block->lines != NULL) {
			fprintf(file, "%s%s\n", MARKDOWN_CODEBLOCK, ext);
			for (size_t j = 0; j < (*block->lines)->size; j++) {
				fprintf(file, "%s\n", (*block->lines)->lines[j]->content);
			}
			fprintf(file, "%s\n", MARKDOWN_CODEBLOCK);
		}
	}
}
//@END
