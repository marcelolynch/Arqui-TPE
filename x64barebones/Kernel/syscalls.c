#include <stdint.h>
#include "video_driver.h"
#include "keyboard_driver.h"

typedef uint64_t (* syscall_ptr)(uint64_t p, uint64_t q, uint64_t r);

uint64_t _write(uint64_t fd, uint64_t str, uint64_t size);
uint64_t _read(uint64_t fd, uint64_t str, uint64_t size);
uint64_t _clrscrn(uint64_t dummy, uint64_t dummy2, uint64_t dummy3);

syscall_ptr sysCalls[] = {
	0, 0, 0,
	_read, //Syscall 3
	_write, //Syscall 4
	_clrscrn //Syscall 5
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
}

uint64_t _clrscrn(uint64_t dummy, uint64_t dummy2, uint64_t dummy3){
	clear_screen();
}