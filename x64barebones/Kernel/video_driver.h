#ifndef __video_driver
#define __video_driver
#include <stdint.h>

void screen_write(char* s, uint64_t size);

void print_positioned(char * s, char f, char c);
void newline();
void clear();
void clear_screen();

#endif