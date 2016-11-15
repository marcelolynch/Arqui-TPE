#ifndef __syscalls_h
#define __syscalls_h
#include <stdint.h>

uint64_t sys_read(int fd, int * buf, int size);
uint64_t sys_write(int fd, char * str, int size);
uint64_t sys_clrscrn();
uint64_t sys_memalloc(uint64_t bytes);
uint64_t sys_send(char * msg);
uint64_t sys_get_msg(char * buf, int max_size);
uint64_t sys_clear_msgs();

#endif