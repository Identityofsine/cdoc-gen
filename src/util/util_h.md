# util.h
#### Custom Memory Allocation Functions
These functions/macros are used to allocate, free, and reallocate memory using the already defined malloc, free, and realloc functions. These functions are just wrappers that make it a bit safer and easier to debug.
```c
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
```
