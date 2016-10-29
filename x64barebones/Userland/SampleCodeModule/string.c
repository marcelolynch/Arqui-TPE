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
