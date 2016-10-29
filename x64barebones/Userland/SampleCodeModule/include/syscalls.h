#ifndef __syscalls_h
#define __syscalls_h
#include <stdint.h>

uint64_t sys_read(int fd, int * buf, int size);
uint64_t sys_write(int fd, char * str, int size);
uint64_t sys_clrscrn();

#endif