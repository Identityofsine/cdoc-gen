# markdown.md
## Write a markdown file
```cpp
void write_markdown(const char* filename, const char* path, Block** blocks, size_t size) {
	FILE *file = fopen(path, "w");
	if (file == NULL) {
		fprintf(stderr, "Error: Could not open file %s\n", path);
		return;
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
			fprintf(file, "%scpp\n", MARKDOWN_CODEBLOCK);
			for (size_t j = 0; j < (*block->lines)->size; j++) {
				fprintf(file, "%s\n", (*block->lines)->lines[j]->content);
			}
			fprintf(file, "%s\n", MARKDOWN_CODEBLOCK);
		}
	}
}
```
