#include "stdlib.h"
#include "syscalls.h"

void * malloc(uint64_t bytes){
	return (void*)sys_memalloc(bytes);
}

void free(void * ptr){
	return;
}

void * realloc(void * ptr, uint64_t size){
	return ptr;
}