#include "syscalls.h"
#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"

#define STDOUT 1
#define STDIN 0
#define BUF_SIZE 100

static char *convert(unsigned int num, int base);

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





void getline(char * buf, int max){
    int c;
    int i = 0;
    while((c = getchar()) != '\n'){
        if(c == '\b'){
            if(i > 0){ //Borro del buffer de comando
            i--;
            putchar(c); //Y de la pantalla
            }

        } else if(i < max - 1){ //Dejo espacio para el 0
            buf[i++] = c; //Agrego al buffer
            putchar(c);
        }
    }

    buf[i] = 0; //Null termination
    return;
}


void printf(char* fmt, ...) 
{ 

	va_list arg; 
    va_start(arg, fmt); 

    vprintf(fmt, arg);

    //Cerrando la lista variable
    va_end(arg); 
} 


void vprintf(char *fmt, va_list arg){
	
	char *traverse; 
    int i; 
    char *s; 


    for(traverse = fmt; *traverse != '\0'; traverse++) 
    { 
        while( *traverse != '%' && *traverse != '\0') 
        { 
            putchar(*traverse);
            traverse++; 
        } 

        if(*traverse == 0)
        	break;

        traverse++; 

        //Module 2: Fetching and executing arguments
        switch(*traverse) 
        { 
            case 'c' : i = va_arg(arg,int);     //Fetch char argument
                        putchar(i);
                        break; 

            case 'd' : i = va_arg(arg,int);         //Fetch Decimal/Integer argument
                        if(i<0) 
                        { 
                            i = -i;
                            putchar('-'); 
                        } 
                        puts(convert(i,10));
                        break; 

            case 'o': i = va_arg(arg,unsigned int); //Fetch Octal representation
                        puts(convert(i,8));
                        break; 

            case 's': s = va_arg(arg,char *);       //Fetch string
                        puts(s); 
                        break; 

            case 'x': i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
                        puts(convert(i,16));
                        break; 
        }   
    } 


}


void sprintf(char* buf, char* fmt, ...) 
{ 
    char *traverse; 
    int i; 
    char *s; 
    char * cnv;

    va_list arg; 
    va_start(arg, fmt); 

    for(traverse = fmt; *traverse != '\0'; traverse++) 
    { 
        while( *traverse != '%' && *traverse != '\0') 
        { 
            *buf = *traverse;

            buf++;
            traverse++; 
        } 

        if(*traverse == 0) 
        	break; //Termine


        traverse++; 

        //Module 2: Fetching and executing arguments
        switch(*traverse) 
        { 
            case 'c' : i = va_arg(arg,int);     //Fetch char argument
                        *(buf++) = (char)i;
                        break; 

            case 'd' : i = va_arg(arg,int);         //Fetch Decimal/Integer argument
                        if(i<0) 
                        { 
                            i = -i;
                           *(buf++) = '-'; 
                        } 
                        cnv = convert(i,10);
                        strncpy(buf, cnv, strlen(cnv));
                        buf += strlen(cnv);
                        break; 

            case 'o': i = va_arg(arg,unsigned int); //Fetch Octal representation
                        cnv = convert(i,8);
                        strncpy(buf, cnv, strlen(cnv));
                        buf += strlen(cnv);
                        break; 

            case 's': s = va_arg(arg,char *);       //Fetch string
                        strncpy(buf, s, strlen(s));
                        buf += strlen(s);
 
                        break; 

            case 'x': i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
                        cnv = convert(i,16);
                        strncpy(buf, cnv, strlen(cnv));
                        buf += strlen(cnv);
                        break; 
        }   
    } 

    *buf = '\0'; //null termination

    //Cerrando la lista variable
    va_end(arg); 
} 




//Scanf con %d, %s
int scanf(char* fmt, ...) 
{ 
    char *traverse;
    unsigned int i; 

    char * str; 
    int * int_ptr;

    int count = 0;

    va_list arg; 
    va_start(arg, fmt); 

    char line[BUF_SIZE];
    char * buf = line;
    char aux[BUF_SIZE];

    getline(line, BUF_SIZE);

    for(traverse = fmt; *traverse != '\0' && *buf != '\0'; traverse++) 
    { 
        while( *traverse != '%'){ //Avanzo hasta el %
            //Si no matchean
            if(*traverse != *buf){ //Dejo de leer, ya no cumple el formato
                  va_end(arg);
                return count; 
            }

            buf++;
            traverse++; 
        } 

        if(*traverse == 0){
            va_end(arg); 
            return count; //Termine
        }

        traverse++; 

        //Module 2: Fetching and executing arguments
        switch(*traverse) { 
            case 'd' :
                        int_ptr = va_arg(arg,int *); //Leo puntero

                        if(*buf != '-' && !isdigit(*buf)){
                            printf("%c", *buf);
                            printf(" char \n");
                            va_end(arg);
                            return count;
                        }
                      

                        aux[0] = *(buf++); //Copio el primero (digito o '-')
                        for(i=1; isdigit(*buf) && i < BUF_SIZE - 1; buf++, i++){ 
                            //Copio el resto de los digitos
                            aux[i] = *buf;
                            printf("%c \n", *buf);
                        }
                        aux[i] = '\0';
                        *int_ptr = atoi(aux);
                        count++;

                        break; 

            case 's' : str = va_arg(arg, char *);       //Fetch string
                      strncpy(str, buf, strlen(buf)); //Copio hasta el final en s
                      count++;
                      va_end(arg); 
                      return count;
                      break; 

        }   
    } 

    //Cerrando la lista variable
    va_end(arg); 
    return count;
} 


static char *convert(unsigned int num, int base) 
{ 
    static char Representation[]= "0123456789ABCDEF";
    static char buffer[50]; 
    char *ptr; 

    ptr = &buffer[49]; 
    *ptr = '\0'; 

    do 
    { 
        *--ptr = Representation[num%base]; 
        num /= base; 
    }while(num != 0); 

    return(ptr); 
}



