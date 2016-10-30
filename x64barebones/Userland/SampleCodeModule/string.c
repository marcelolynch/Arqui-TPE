#include "string.h"


//Devuelve la longitud de la cadena
int strlen(char * s){
	int c = 0;
	while(*(s++) != 0){
		c++;
	}

	return c;
}


//Compara 2 cadenas, devuelve 0 si son iguales
int strcmp(char * s1, char * s2){
	while(*s1 && *s2 && *s1 == *s2){
		s1++;
 		s2++;
	}
	return *s1 - *s2;
}


int strncmp(char * s, char * t, unsigned int n) {
	int i;
	for (i = 0; i < n-1 && s[i] != '\0' && t[i] != '\0' && s[i] == t[i]; i++)
		;
	return s[i]-t[i];
}

//Devuelve TRUE si start es subcadena inicial de str, FALSE (0) 
//si esto no pasa
int starts_with(char * str, char * start){
	while(*str != 0 && *start != 0){
		if(*(str++) != *(start++)){
			return 0;
		}
	}
	return *start == 0;
}


char * strcpy ( char * destination, char * source){
	return strncpy(destination, source, strlen(source) + 1);
}


char * strncpy ( char * destination, char * source, int size){
	int i = 0;
	char * ret = destination;
	while(*source && i < size){
		*(destination++) = *(source++);
		i++;
	}
	*destination = '\0';
	return ret;
}

int strchr(char*str, char c){
	while(*str){
		if(*str++ == c)
			return 1;
	}
	return 0;
}