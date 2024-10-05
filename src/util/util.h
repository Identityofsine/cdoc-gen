#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


static bool fxMemAllocMessages = false; 

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
  free(ptr);
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
  return realloc(ptr, size);
}

#define fxMemRealloc(ptr, size) _fxMemRealloc(ptr, size, __FUNCTION__)

#endif
