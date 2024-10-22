#include "../doc/doc.h"


//@SECTION
//@TITLE Markdown Macros 
#ifndef MARKDOWN_H1
#define MARKDOWN_H1 "#"
#endif

#ifndef MARKDOWN_H2
#define MARKDOWN_H2 "##"
#endif

#ifndef MARKDOWN_H3
#define MARKDOWN_H3 "###"
#endif

#ifndef MARKDOWN_H4
#define MARKDOWN_H4 "####"
#endif

#ifndef MARKDOWN_CODEBLOCK
#define MARKDOWN_CODEBLOCK "```"
#endif
//@END

//@SECTION
//@TITLE Markdown Definitions
bool write_markdown(const char* source_file, const char* filename, const char* path, Block** blocks, size_t size);
const char** write_markdown_links(List* paths); 
void write_markdown_readme(List* paths, const char* root);
//@END


