#include "stdio.h"
#include "string.h"
#include "syscalls.h"
#define BUF_SIZE 512


void shell();
void getCommand();
void processCommand();

static char cmd_buffer[BUF_SIZE];



int main() {
	shell();
	return 0;
}


void shell(){
	sys_clrscrn();

	puts("Bienvenidos a la shell que hace echo y clear\n");

	while(1){
		putchar('>');
		getCommand();
		processCommand();
	}
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

		} else {
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
	} else{
		puts("\nNo such command\n");
	}
}