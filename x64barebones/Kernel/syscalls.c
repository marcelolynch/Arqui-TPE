#include <stdint.h>
#include "video_driver.h"
#include "keyboard_driver.h"
#include <rtl.h>

typedef uint64_t (* syscall_ptr)(uint64_t p, uint64_t q, uint64_t r);


uint64_t _write(uint64_t fd, uint64_t str, uint64_t size);
uint64_t _read(uint64_t fd, uint64_t str, uint64_t size);
uint64_t _clrscrn(uint64_t dummy, uint64_t dummy2, uint64_t dummy3);
uint64_t _memalloc(uint64_t size, uint64_t dummy2, uint64_t dummy3);
uint64_t _send_msg(uint64_t msg, uint64_t dst, uint64_t dummy);
uint64_t _get_msg(uint64_t buf, uint64_t msg_info, uint64_t max_size);
uint64_t _send_msg(uint64_t msg, uint64_t dst, uint64_t dummy);
uint64_t _send_broadcast(uint64_t msg, uint64_t dummy1, uint64_t dummy2);
uint64_t _clear_msgs();

syscall_ptr sysCalls[] = {
	0, 0, 0,
	_read, //Syscall 3
	_write, //Syscall 4
	_clrscrn, //Syscall 5
	_memalloc, //syscall 6
	_send_msg, //syscall 7
	_send_broadcast, //syscall 8
	_get_msg, //syscall 9
	_clear_msgs  //syscall 10
};




uint64_t syscallDispatcher(int code, uint64_t param1, uint64_t param2, uint64_t param3){
	return sysCalls[code](param1, param2, param3);
}


uint64_t _write(uint64_t fd, uint64_t str, uint64_t size){
	if(fd == 1){
		screen_write((char*)str, size);
	}
}

uint64_t _read(uint64_t fd, uint64_t buf, uint64_t count){
	while(count-- > 0){
		*(int*)buf = getKey();
		buf++;
	}
	return 0;
}

uint64_t _clrscrn(uint64_t dummy, uint64_t dummy2, uint64_t dummy3){
	clear_screen();
}

static void * mem_pointer = (void*)0x1000000; //Doy memoria a partir del 10mo mega porque pinta
uint64_t _memalloc(uint64_t size, uint64_t dummy2, uint64_t dummy3){
	void * ptr = mem_pointer;
	mem_pointer += size; 
	return (uint64_t)ptr;
}




uint64_t _get_msg(uint64_t buf, uint64_t msg_info, uint64_t max_size){
	return rtl_next_msg((char*)buf, (void*)msg_info ,max_size);
}


uint64_t _send_msg(uint64_t msg, uint64_t dst, uint64_t dummy){
	rtl_send((char*)msg, (uint8_t)dst);
	return 0;
}

uint64_t _send_broadcast(uint64_t msg, uint64_t dummy1, uint64_t dummy2){
	rtl_send((char*)msg, -1);
	return 0;
}


uint64_t _clear_msgs(uint64_t dummy1, uint64_t dummy2, uint64_t dummy3){
	rtl_clear_msgs();
	return 0;
}