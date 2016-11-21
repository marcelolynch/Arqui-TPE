#ifndef __stdlib_h
#define __stdlib_h
#include <stdint.h>

#ifndef NULL
#define NULL 0
#endif


void * malloc(uint64_t bytes);

void free(void * ptr);

void * realloc(void * ptr, uint64_t size);

int atoi(char *str);

void * myalloc(uint64_t size);

char* convertDay(int i);

char* convertMonth(int i);
#endif