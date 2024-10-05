#include <stdlib.h>
#ifndef DELIM

#ifdef _WIN32
#define DELIM 0x0d

#else
#define DELIM 0x0a

#endif

#endif

#ifndef GET_DELIM

static inline char *char_to_string(char c) {
  char *str = malloc(2);
  str[0] = c;
  str[1] = '\0';
  return str;
};

#define DELIM_STRING(x) char_to_string(x)

#define GET_DELIM char_to_string(DELIM)

#endif
