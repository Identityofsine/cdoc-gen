# markdown.md
## Write a markdown file
```c
void write_markdown(const char* source_file, const char* filename, const char* path, Block** blocks, size_t size) {
	FILE *file = fopen(path, "w");
	if (file == NULL) {
		fprintf(stderr, "Error: Could not open file %s\n", path);¬jX
		return;›Âž~
	}Âž~
	//strip the path from the source file
	char *source = strrchr(source_file, '/');A¬jX
	if (source == NULL) {
		source = source_file;
	} else {›Âž~
		source++;Âž~
	}Âž~
	//get the extension of the source file
	char *ext = strrchr(source, '.');
	ext = ext + 1;	
	if(strcmp(ext, "h") == 0) {
		ext = "c";ž~
	} Âž~
	fprintf(file, "%s %s\n", MARKDOWN_H1, filename);TB¬jX
	// Write the blocks to the file
	for (size_t i = 0; i < size; i++) {
		Block *block = blocks[i];¬jX
		if (block->title != NULL) {X
			fprintf(file, "%s %s\n", MARKDOWN_H2 ,block->title->content);UB¬jX
		}Âž~
		if (block->desc != NULL) {jX
			for (size_t j = 0; j < block->desc->size; j++) {
				fprintf(file, "%s\n", block->desc->lines[j]->content);
			}
		}Âž~
		if (block->lines != NULL) {X
			fprintf(file, "%s%s\n", MARKDOWN_CODEBLOCK, ext);
			for (size_t j = 0; j < (*block->lines)->size; j++) {
				fprintf(file, "%s\n", (*block->lines)->lines[j]->content);
			}ž~
			fprintf(file, "%s\n", MARKDOWN_CODEBLOCK);X
		}Âž~
}
```
