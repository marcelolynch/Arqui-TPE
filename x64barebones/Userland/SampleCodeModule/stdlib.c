#include "stdlib.h"
#include "syscalls.h"
#include "ctype.h"

void * malloc(uint64_t bytes){
	void * ret = (void*)sys_memalloc(bytes);
	return ret;
}

void free(void * ptr){
	return;
}

void * realloc(void * ptr, uint64_t size){
	return ptr;
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