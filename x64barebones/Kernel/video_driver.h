#ifndef __video_driver
#define __video_driver
#include <stdint.h>


int strlen(char * s);
void print(char * s);
void clear_line();

void screen_write(char* s, uint64_t size);

void print_positioned(char * s, char f, char c);
void newline();
void clear();

#endif