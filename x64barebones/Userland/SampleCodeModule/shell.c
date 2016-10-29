int strlen(char * s);
void puts(char * s, int size);

void shell(){
while(1){
	char * s = "Hola mundo";
	puts(s, strlen(s));
}
}