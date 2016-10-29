#define STDOUT 1
void sys_read(int fd, int * buf, int size);
void sys_write(int fd, char * str, int size);
int strlen(char*s);

int getchar(){
	int c = 0;
	do{
		sys_read(0, &c, 1);
	}while(c == 0);

	return c;

}

void putchar(char c){
	sys_write(STDOUT, &c, 1);
}


void puts(char * s){
	sys_write(STDOUT, s, strlen(s));
}