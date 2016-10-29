#include "syscalls.h"

#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_CLRSCRN 5

uint64_t _syscall(uint64_t code, uint64_t param1, uint64_t param2, uint64_t param3);

//System call READ
uint64_t sys_read(int fd, int * buf, int size){
	return _syscall(SYS_READ, fd, (uint64_t)buf, (uint64_t)size);
}

//System call WRITE
uint64_t sys_write(int fd, char * str, int size){
	return _syscall(SYS_WRITE, fd, (uint64_t)str, (uint64_t)size);
}

//System call para borrar la pantalla
uint64_t sys_clrscrn(){
	return _syscall(SYS_CLRSCRN, 0,0,0);
}
