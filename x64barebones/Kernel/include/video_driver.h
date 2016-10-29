#ifndef __video_driver
#define __video_driver

int strlen(char * s);
void print(char * s);
void print_formatted(char* s, char fmt);
void clear_line();

void screen_write(char * chars, size_t size);

void print_positioned(char * s, char f, char c);
void newline();
void clear();

#endif