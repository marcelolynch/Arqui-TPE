#include "getnum.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "syscalls.h"
#define MAX_SIZE 512
#define MAX_MSG_SIZE 1024

static int processChatCommand(char * buf);
static void getChatCommand();


static char cmd_buffer[MAX_SIZE];


void chat(){
	char buffer[1000];
	int active = 1;
	sys_clrscrn();
	sys_clear_msgs();
	printf("   Bienvenido a este humilde chat!\n \t 'r' para recibir nuevos mensajes \n\t 'send <tu mensaje>' para mandar un mensaje publico\n");
	while(active){
		putchar('\n');
		putchar('>');
		getChatCommand();
		active = processChatCommand(buffer);
	}

}



static void getChatCommand(){
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



static int processChatCommand(char * buf){
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
		printf("CHAT!\n \t 'r' para recibir nuevos mensajes \n\t 'send <tu mensaje>' para mandar un mensaje publico\n");
	}

	else if(starts_with(cmd_buffer, "send ")){
		sys_send(cmd_buffer + 5);
	}	

	else if(strcmp("r", cmd_buffer) == 0){
		msg_desc msg_info;
		int b;
		if((b = sys_get_msg(buf, &msg_info, MAX_MSG_SIZE)) == -1){
			printf("\nNo new messages :(\n");
		}else{
			do{
			printf("\n Nuevos mensajes:\n");
			printf("%s message from user %d: %s \n", 
						msg_info.is_broadcast ? "Public" : "private", 
						msg_info.user, 
						buf);
			
			}while((b = sys_get_msg(buf, &msg_info, MAX_MSG_SIZE)) != -1);
		}

	}

	else if(strcmp("exit", cmd_buffer) == 0){
		return 0;
	}
	else{
		printf("\nInvalid command \n");
	}
	return 1;
}