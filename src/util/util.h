#pragma once
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


static bool fxMemAllocMessages = false; 

//@SECTION
//@TITLE Custom Memory Allocation Functions
//@DESC These functions/macros are used to allocate, free, and reallocate memory using the already defined malloc, free, and realloc functions. These functions are just wrappers that make it a bit safer and easier to debug.
#ifndef fxMemAlloc

inline static void *_fxMemAlloc(size_t size, const char *source) {
	if (fxMemAllocMessages) {
		if (source) {
			printf("[%s] Allocating %lu bytes \n", source, size);
		} else {
			printf("[UNKNOWN] Allocating %lu bytes\n", size);
		}
	}
  return malloc(size);
}

#define fxMemAlloc(size) _fxMemAlloc(size, __FUNCTION__)

#endif

#ifndef fxMemFree

inline static void _fxMemFree(void *ptr, const char *source) {
	if (fxMemAllocMessages) {
		if (source) {
			printf("[%s] Freeing memory\n", source);
		} else {
			printf("[UNKNOWN] Freeing memory\n");
		}
	}
	if (ptr) {
  	free(ptr);
	}
	else {
		printf("Error: Attempted to free NULL pointer (SAVED YOU... %s)\n", source);
	}
}

#define fxMemFree(ptr) _fxMemFree(ptr, __FUNCTION__)

#endif

#ifndef fxMemRealloc

inline static void *_fxMemRealloc(void *ptr, size_t size, const char *source) {
	if (fxMemAllocMessages) {
		if (source) {
			printf("[%s] Reallocating %lu bytes\n", source, size);
		} else {
			printf("[UNKNOWN] Reallocating %lu bytes\n", size);
		}
	}
	if (ptr == NULL) {
		return malloc(size);
	}
  return realloc(ptr, size);
}

#define fxMemRealloc(ptr, size) _fxMemRealloc(ptr, size, __FUNCTION__)

#endif

//@END

inline static const char* strip_whitespace_from_start(const char *str) {
	while (*str == ' ' || *str == '\t' || *str == 0x20) {
		str++;
	}
	return str;
}

#ifndef CONCAT_STRING 

inline static const char *concat_string(const char *a, const char *b) {
	if (a == NULL) {
		printf("Error: First string is NULL\n");
		return b;
	}
	if (b == NULL) {
		printf("Error: Second string is NULL\n");
		return a;
	}
	char *result = malloc(strlen(a) + strlen(b) + 1);
	strcpy(result, a);
	strcat(result, b);
	return result;
}
#define CONCAT_STRING(a, b) concat_string(a, b)


#endif


#ifndef STRIP_DOUBLE_SLASHES

inline static const char *strip_double_slashes(const char *a) {
	if (a == NULL) {
		printf("Error: First string is NULL\n");
		return "";
	}

	char* copy = (char*)fxMemAlloc(strlen(a) + 1);
	strcpy(copy, a);

	//strip double slashes, leave one and remove the rest
	for (size_t i = 0; i < strlen(a); i++) {
		if (copy[i] == '/' && copy[i + 1] == '/') {
			size_t i_cpy = i;
			while(copy[i + 1] == '/') {
				i++;
			}
			memmove(copy + i_cpy + 1, copy + i + 1, strlen(copy) - i);
		} 
	}
	return copy;
}

#define STRIP_DOUBLE_SLASHES(a) strip_double_slashes(a)

#endif

