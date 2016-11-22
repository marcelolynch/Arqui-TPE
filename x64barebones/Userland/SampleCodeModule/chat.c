#include "getnum.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "syscalls.h"
#define MAX_SIZE 1024
#define MAX_MSG_SIZE 1024

#define SEND_CMD_OFFSET 2
#define BCAST_CMD_OFFSET 2


static int processChatCommand(char * buf);
static void getChatCommand();


static char cmd_buffer[MAX_SIZE];

static int connected_users[255];
static int myID;

static void printHelp();



void chat(){
	char buffer[1000];
	int active = 1;
	sys_clrscrn();
	sys_clear_msgs();
	sys_connect();
	myID = sys_get_network_id();
	printf("Bienvenido a HumbleChat!\n");
	printHelp();
	while(active){
		putchar('\n');
		putchar('>');
		getChatCommand();
		active = processChatCommand(buffer);
	}

	sys_disconnect();
	sys_clrscrn();

}


static void printHelp(){
	printf("Su numero de usuario es #%d\n", myID);
	printf("\n\nLos comandos del chat son:\n");
	printf("\t'users' para ver los usuarios conectados\n");
	printf("\t'r' para recibir nuevos mensajes \n");
	printf("\t's <id> <tu mensaje>' para mandarle un mensaje al usuario <id>\n\t\t(el id debe ser un numero entre 0 y 255)\n");
	printf("\t'b <tu mensaje>' para mandar un mensaje publico\n");
	printf("\t'clear' para limpiar la pantalls\n");	
	printf("\t'help' para volver a mostrar esta ayuda\n");	

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
	
	else if(strcmp("clear", cmd_buffer) == 0){
		sys_clrscrn();
		printf("HumbleChat 2.0 \n Escribi 'help' para mostrar los comandos \n");
	}

	else if(starts_with(cmd_buffer, "s ")){
		int user;
		int digits_offset;
		putchar('\n');
		if(decode_send(&user, &digits_offset)){
			if(user > 0xFF){
				printf("No existe ese usuario (los usuarios van de 0 a 255)\n");
			}else if(user == myID){
				printf("No puede enviarse un mensaje a usted mismo\n");
			}
			else{
				int i;
				int active = 0; 
				int count = sys_get_active_users(connected_users);
				
				for(i = 0; i < count ; i++){
					if(user == connected_users[i]){
						//El usuario esta efectivamente conectado
						active = 1;
					}
				}
				if(active){
					printf("Se envio su mensaje al usuario #%d\n", user);
					sys_send(cmd_buffer + SEND_CMD_OFFSET + digits_offset, user);
				}else{
					printf("El usuario #%d no esta conectado\n", user);
				}
			}
		}
		else{
			printf("\nComando invalido\n");
		}
	}	

	else if(starts_with(cmd_buffer, "b ")){
		sys_broadcast(cmd_buffer + BCAST_CMD_OFFSET);
	}	

	else if(strcmp("r", cmd_buffer) == 0){
		msg_desc msg_info;
		int b;
		if((b = sys_get_msg(buf, &msg_info, MAX_MSG_SIZE)) == -1){
			printf("\nNo hay mensajes nuevos :(\n");
		}else{
			do{
			printf("\n Nuevos mensajes:\n");
			printf("[%d/%d/%d-%d%s%d]\n", msg_info.time.day, msg_info.time.month, msg_info.time.year, msg_info.time.hour, ((msg_info.time.min < 10)? ":0" : ":"),msg_info.time.min);
			printf("[Mensaje %s del usuario #%d]  %s \n", 
						msg_info.is_broadcast ? "publico" : "privado", 
						msg_info.user, 
						buf);
			
			}while((b = sys_get_msg(buf, &msg_info, MAX_MSG_SIZE)) != -1);
		}

	}

	else if(strcmp("exit", cmd_buffer) == 0){
		return 0;
	
	} 
	else if(strcmp("users", cmd_buffer) == 0){
		int i;
		int count = sys_get_active_users(connected_users);
		if(count > 0){
			printf("\nUsuarios conectados: \n");
		}else{
			printf("\nNo hay usuarios conectados\n");
		}
		for(i = 0; i < count; i++){
			printf("Usuario #%d\n", connected_users[i]);
		}

	}

	else if(strcmp("help", cmd_buffer) == 0){
		printHelp();
	}

	else{
		printf("\nComando invalido\n");
	}
	return 1;
}


int decode_send(int * user, int * offset){
	*user = 0;
	*offset = 0;

	char * c = cmd_buffer + SEND_CMD_OFFSET;
	while(*c != ' '){
		if(!isdigit(*c))
			return 0; //Mal formado
 		*user = (*user)*10 + (*c - '0');
 		(*offset)++;
 		c++;
	}
	(*offset)++; //El espacio
	return 1;
}