# markdown.md
## Write a markdown file
```c
void write_markdown(const char* source_file, const char* filename, const char* path, Block** blocks, size_t size) {
	FILE *file = fopen(path, "w");
	if (file == NULL) {
		fprintf(stderr, "Error: Could not open file %s\n", path);Ã™V
		return;«ÞB{
	}ÞB{
	//strip the path from the source file
	char *source = strrchr(source_file, '/');Ã™V
	if (source == NULL) {
		source = source_file;
	} else {«ÞB{
		source++;B{
	}ÞB{
	//get the extension of the source file
	char *ext = strrchr(source, '.');
	ext = ext + 1;	
	if(strcmp(ext, "h") == 0) {
		ext = "c";B{
	} B{
	if (size == 0) {ÖÃ™V
		return;«ÞB{
	}ÞB{
	fprintf(file, "%s %s\n", MARKDOWN_H1, filename);ÓÃ™V
	// Write the blocks to the filepÕÃ™V
	for (size_t i = 0; i < size; i++) {
		Block *block = blocks[i];
		if (block->title != NULL) {V
			fprintf(file, "%s %s\n", MARKDOWN_H2 ,block->title->content);ÔÃ™V
		}B{
		if (block->desc != NULL) {
			for (size_t j = 0; j < block->desc->size; j++) {
				fprintf(file, "%s\n", block->desc->lines[j]->content);
			}B{
		}B{
		if (block->lines != NULL) {
			fprintf(file, "%s%s\n", MARKDOWN_CODEBLOCK, ext);
			for (size_t j = 0; j < (*block->lines)->size; j++) {
				fprintf(file, "%s\n", (*block->lines)->lines[j]->content);
			}
			fprintf(file, "%s\n", MARKDOWN_CODEBLOCK);
		}
}
```
