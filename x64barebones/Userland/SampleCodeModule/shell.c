#define BUF_SIZE 512

int strlen(char * s);
int getchar();
void putchar(char c);
void puts(char * s);
int strcmp(char * s1, char * s2);
int starts_with(char * s, char * start);

static char cmd_buffer[BUF_SIZE];

void shell(){
	void getCommand();
	void processCommand();
	puts("Welcome to Userland\n");

	while(1){
		putchar('>');
		getCommand();
		processCommand();
		putchar('\n');
	}
}


void getCommand(){
	int c;
	int i = 0;
	while((c = getchar()) != '\n'){
		putchar(c);
		cmd_buffer[i++] = c;
	}
	cmd_buffer[i] = 0;
	return;
}


void processCommand(){
	if(starts_with(cmd_buffer, "echo")){
		putchar('\n');
		puts(cmd_buffer + 5);
		putchar('\n');
	}
	else if(strcmp("clear", cmd_buffer) == 0){
		puts("clear\n");
	}
}