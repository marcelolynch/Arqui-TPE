#include "getnum.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "syscalls.h"
#define MAX_SIZE 512

#define SHELLMSG  " ___| |__   ___| | |  ___\n/ __| '_ \\ / _ \\ | | / __|\n\\__ \\ | | |  __/ | || (__ \n|___/_| |_|\\___|_|_(_)___|\n"

void fanorona();
void shell();
void chat();


static void getCommand();
static void processCommand();

static char cmd_buffer[MAX_SIZE];



int main() {
	shell();
	return 0;
}

static int run;
void shell(){

//	int a= 100, b;

	sys_clrscrn();
	puts(SHELLMSG);

	run = 1;
	while(run){
		putchar('>');
		getCommand();
		processCommand();
	}
	return;
}


static void getCommand(){
	int c;
	int i = 0;
	while((c = getchar()) != '\n'){
		if(c == '\b'){
			if(i > 0){ //Borro del buffer de comando
			i--;
			putchar(c); //Y de la pantalla
			}

		} else if(i < MAX_SIZE - 1){ //Dejo espacio para el 0
			cmd_buffer[i++] = c; //Agrego al buffer
			putchar(c);
		}
	}
	cmd_buffer[i] = 0; //Null termination
	return;
}


static void processCommand(){
	if(cmd_buffer[0] == 0){
		putchar('\n');
	}
	else if(starts_with(cmd_buffer, "echo ")){
		putchar('\n');
		puts(cmd_buffer + 5);
		putchar('\n');
	}
	else if(strcmp("clear", cmd_buffer) == 0){
		sys_clrscrn();
	}
	else if(strcmp("help", cmd_buffer) == 0){
		help();
	}
	else if(strcmp("fanorona", cmd_buffer) == 0){
		sys_clrscrn();
		fanorona();
		sys_clrscrn();
	}
	else if(strcmp(cmd_buffer, "chat") == 0){
		chat();
		sys_clrscrn();
	}
	else if(strcmp("exit", cmd_buffer) == 0){
		run = 0;
	}
	else if(strcmp("2048", cmd_buffer) == 0){
		sys_clrscrn();
		dosMilCuarentaYOcho();
		sys_clrscrn();
	}
	else if(strcmp("ran", cmd_buffer) == 0){
		printf("\n%d\n",rand());
	}
	else if(strcmp("time", cmd_buffer) == 0){
		printf("\n %d:%d:%d del %s ", getFromClock(2),getFromClock(1),getFromClock(0),convertDay(getFromClock(3)));
		printf("%d de %s de %d \n", getFromClock(4), convertMonth(getFromClock(5)), 2000 + getFromClock(6));
	}
	else
	{
		printf("\nComando invalido, escriba 'help' para ver los comandos disponibles\n");
	}
}