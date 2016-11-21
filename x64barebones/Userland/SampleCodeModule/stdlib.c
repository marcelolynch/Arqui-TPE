#include "stdlib.h"
#include "syscalls.h"
#include "ctype.h"

#define RAND_MAX 32767

void* malloc(uint64_t bytes){
	void * ret = (void*)sys_memalloc(bytes);
	return ret;
}

static void * mem_pointer = (void*)0x1000000; //Doy memoria a partir del 10mo mega porque pinta
void * myalloc(uint64_t size){
	void * ptr = mem_pointer;
	mem_pointer += size; 
	return ptr;
}

void free(void * ptr){
	return;
}

void* realloc(void * ptr, uint64_t size){
	return ptr;
}

unsigned int rand(void){
	return sys_getAmountOfTicks() % RAND_MAX;
}

int atoi(char *str){
	int answer = 0;
	int neg = 1;
	if(*str == '-'){
		neg = -1;
		str++;
	}
	while(*str){
		if(!isdigit(*str))
			return 0;
		answer = 10*answer + (*str - '0');
		str++;
	}
	
	return neg*answer;
}