#pragma once
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


static bool fxMemAllocMessages = true; 

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

inline static const char* strip_whitespace_from_start(const char *str) {
	while (*str == ' ' || *str == '\t' || *str == 0x20) {
		str++;
	}
	return str;
}

#ifndef CONCAT_STRING 

inline static const char *concat_string(const char *a, const char *b) {
	char *result = malloc(strlen(a) + strlen(b) + 1);
	strcpy(result, a);
	strcat(result, b);
	return result;
}
#define CONCAT_STRING(a, b) concat_string(a, b)

#endif

