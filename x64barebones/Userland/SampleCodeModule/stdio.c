#include "syscalls.h"
#include "string.h"
#include <stdarg.h>

#define STDOUT 1
#define STDIN 0

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




void printf(char* fmt, ...) 
{ 
    char *traverse; 
    unsigned int i; 
    char *s; 

    va_list arg; 
    va_start(arg, fmt); 

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

    //Cerrando la lista variable
    va_end(arg); 
} 


void sprintf(char* buf, char* fmt, ...) 
{ 
    char *traverse; 
    unsigned int i; 
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