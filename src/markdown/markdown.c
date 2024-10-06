#include "markdown.h"
#include "../util/util.h"
#include "../file/file.h"
#include <stdio.h>

//@BLOCK
//@TITLE Write a markdown file
//@DESC Writes a markdown file from a list of blocks.
void write_markdown(const char* source_file, const char* filename, const char* path, Block** blocks, size_t size) {

	if (size == 0) {
		printf("WARNING: %s has no blocks to write\n", source_file);
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
	char *modi = NULL;

	if(strcmp(ext, "h") == 0 || strcmp(ext, "c") == 0) {
		modi = ext;
		ext = "c";
	} 

	// modify the filename if c and h files
	
	if (modi != NULL) {
		path = remove_ext(path);
		path = CONCAT_STRING(path, "_");
		path = CONCAT_STRING(path, modi);
		path = CONCAT_STRING(path, ".md");
	}

	FILE *file = fopen(path, "w");
	if (file == NULL) {
		fprintf(stderr, "Error: Could not open file %s\n", path);
		return;
	}

	fprintf(file, "%s %s\n", MARKDOWN_H1, source);

	// Write the blocks to the file
	for (size_t i = 0; i < size; i++) {
		Block *block = blocks[i];

		if (block == NULL) {
			printf("Error: Block is null\n");
			continue;
		}

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
}
//@END
