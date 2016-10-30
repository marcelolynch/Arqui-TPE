#ifndef __stdio_h
#define __stdio_h
#include <stdarg.h>
#ifndef NULL
#define NULL 0
#endif

int getchar();
void putchar(char c);
void puts(char * s);
void printf(char* fmt, ...);
void sprintf(char * buf, char *fmt, ...);
void vprintf(char *fmt, va_list arg);

#endif