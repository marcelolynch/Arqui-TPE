#ifndef __syscalls_h
#define __syscalls_h
#include <stdint.h>


typedef struct{
	int is_broadcast;
	int user;
	struct{
		char day;
		char month;
		char year;
		char hour;
		char min;
	} time;
} msg_desc;


uint64_t sys_read(int fd, int * buf, int size);
uint64_t sys_write(int fd, char * str, int size);
uint64_t sys_clrscrn();
uint64_t sys_memalloc(uint64_t bytes);
uint64_t sys_send(char * msg, char user);
uint64_t sys_broadcast(char * msg);
uint64_t sys_get_msg(char * buf, msg_desc* msg_info, int max_size);
uint64_t sys_clear_msgs();
uint64_t sys_connect();
uint64_t sys_disconnect();
uint64_t sys_get_active_users(int * vec);

#endif