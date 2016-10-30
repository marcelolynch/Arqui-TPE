#ifndef __string_h
#define __string_h

int strcmp(char * s1, char * s2);
int starts_with(char * s, char * start);
int strlen(char * s);
char * strcpy ( char * destination, char * source );
char * strncpy ( char * destination, char * source, int size);
#endif