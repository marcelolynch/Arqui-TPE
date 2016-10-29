#include "syscalls.h"
#include "string.h"

#define STDOUT 1
#define STDIN 0


//Lee un caracter de entrada estandar y lo devuelve
int getchar(){
	int c = 0;
	do{
		sys_read(STDIN, &c, 1);
	}while(c == 0);

	return c;

}

//Imprime un caracter en la salida estandar
void putchar(char c){
	sys_write(STDOUT, &c, 1);
}


//Imprime una cadena a salida estandar
void puts(char * s){
	sys_write(STDOUT, s, strlen(s));
}