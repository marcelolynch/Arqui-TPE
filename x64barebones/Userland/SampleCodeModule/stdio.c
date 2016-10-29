#define STDOUT 1
#include "syscalls.h"
#include "string.h"

int getchar(){
	int c = 0;
	do{
		sys_read(0, &c, 1);
	}while(c == 0);

	return c;

}

void putchar(char c){
	sys_write(STDOUT, &c, 1);
}


void puts(char * s){
	sys_write(STDOUT, s, strlen(s));
}