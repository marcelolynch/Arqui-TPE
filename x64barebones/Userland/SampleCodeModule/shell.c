#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "syscalls.h"
#define MAX_SIZE 512

#define SHELLMSG  " ___| |__   ___| | |  ___\n/ __| '_ \\ / _ \\ | | / __|\n\\__ \\ | | |  __/ | || (__ \n|___/_| |_|\\___|_|_(_)___|\n"


void shell();
void getCommand();
void processCommand();

static char cmd_buffer[MAX_SIZE];



int main() {
	shell();
	return 0;
}

static int run;
void shell(){

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

void getCommand(){
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


void processCommand(){
	if(starts_with(cmd_buffer, "echo ")){
		putchar('\n');
		puts(cmd_buffer + 5);
		putchar('\n');
	}
	else if(strcmp("clear", cmd_buffer) == 0){
		sys_clrscrn();
	} else if(strcmp("exit", cmd_buffer)== 0){

		run = 0;
	}
	else{
		puts("\nNo such command\n");
	}
}