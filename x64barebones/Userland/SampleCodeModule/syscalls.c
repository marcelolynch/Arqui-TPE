#include "syscalls.h"


#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_CLRSCRN 5
#define SYS_MEMALLOC 6
#define SYS_SEND_MSG 7
#define SYS_GET_MSG 8
#define SYS_CLEAR_MSGS 9



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

// Para alocar memoria
uint64_t sys_memalloc(uint64_t bytes){
	return _syscall(SYS_MEMALLOC, bytes, 0, 0);
}

// System call para enviar un mensaje a la red
uint64_t sys_send(char * msg){
	return _syscall(SYS_SEND_MSG, (uint64_t)msg, 0, 0);
}

//Syscall para recuperar el siguiente mensaje no leido de la cola
//Devuelve -1 si no hay mensajes
uint64_t sys_get_msg(char * buf, int max_size){
	return _syscall(SYS_GET_MSG, (uint64_t)buf, max_size, 0);
}


//Resetea la cola de mensajes
uint64_t sys_clear_msgs(){
	return _syscall(SYS_CLEAR_MSGS, 0, 0, 0);
}