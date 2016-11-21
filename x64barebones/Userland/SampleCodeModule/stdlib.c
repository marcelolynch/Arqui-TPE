#include "stdlib.h"
#include "syscalls.h"
#include "ctype.h"

#define RAND_MAX 32767

void* malloc(uint64_t bytes){
	void * ret = (void*)sys_memalloc(bytes);
	return ret;
}

static void * mem_pointer = (void*)0x1000000; //Doy memoria a partir del 10mo mega porque pinta
void * myalloc(uint64_t size){
	void * ptr = mem_pointer;
	mem_pointer += size; 
	return ptr;
}

void free(void * ptr){
	return;
}

void* realloc(void * ptr, uint64_t size){
	return ptr;
}

unsigned int rand(void){
	return sys_getAmountOfTicks() % RAND_MAX;
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

int getFromClock(int i){
	return sys_get_rtc_data(i);
}

char* convertDay(int i){
	switch(i){
		case 1:
			return "Domingo";
		case 2:
			return "Lunes";
		case 3:
			return "Martes";
		case 4:
			return "Miercoles";
		case 5:
			return "Jueves";
		case 6:
			return "Viernes";
		case 7:
			return "Sabado";
		default:
			return " ";
	}
}

char* convertMonth(int i){
	switch(i){
		case 1:
			return "Enero";
		case 2:
			return "Febrero";
		case 3:
			return "Marzo";
		case 4:
			return "Abril";
		case 5:
			return "Mayo";
		case 6:
			return "Junio";
		case 7:
			return "Julio";
		case 8:
			return "Agosto";
		case 9:
			return "Septiembre";
		case 10:
			return "Octubre";
		case 11:
			return "Noviembre";
		case 12:
			return "Diciembre";
		default:
			return " ";
	}
}