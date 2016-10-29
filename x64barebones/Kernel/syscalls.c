#include <stdint.h>
#include "video_driver.h"

typedef uint64_t (* sysCall_ptr)(uint64_t, uint64_t, uint64_t);

uint64_t _write(uint64_t fd, uint64_t str, uint64_t size);

sysCall_ptr sysCalls[] = {
	0, 0, 0,
	0, //Syscall 4
	_write, //Syscall 5
	//
};

uint64_t syscallDispatcher(int code, uint64_t param1, uint64_t param2, uint64_t param3){
	return sysCalls[code](param1, param2, param3);
}


uint64_t _write(uint64_t fd, uint64_t str, uint64_t size){
	if(fd == 1){
		screen_write((char*)str, size);
	}
}