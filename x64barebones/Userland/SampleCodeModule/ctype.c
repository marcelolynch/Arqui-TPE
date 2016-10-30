#include "ctype.h"

char toupper(char c){
	if(islower(c)){
		return c += 'A' - 'a';
	}
	else return c;
}

char tolower(char c){
	if(c >= 'A' && c <= 'Z'){
		return c += 'a' - 'A';
	}
	else return c;
}


int islower(char c){
	return c >= 'a' && c <= 'z';
}

int isupper(char c){
 	return c >= 'A' && c <= 'Z';
}


int isalpha(char c){
	return islower(c) || isupper(c);
}


int isdigit(char c){
	return c >= '0' && c <= '9';
}

int isalphanum(char c){
	return isalpha(c) || isdigit(c);
}

int isspace(char c){
	return c == ' ' || c == '\t' || c == '\n' 
			|| c == '\v' || c == '\f' || c == '\r';
}