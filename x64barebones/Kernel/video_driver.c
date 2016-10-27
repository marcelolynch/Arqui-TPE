static char * video =  (char *)0xB8000;
static char * currentpos = (char *)0xB8000;
#define VCOLS 80
#define VROWS 25
#define LINE_BYTES 160
#define COL_BYTES 50
#define DEF_FORMAT 0x0f
#include "video_driver.h"

int strlen(char * s){
	int c = 0;
	while(*(s++) != 0){
		c++;
	}

	return c;
}

void clear_line(){
	currentpos -= (currentpos-video)%LINE_BYTES;
}

void newline(){
	int padding = LINE_BYTES - (currentpos - video)%LINE_BYTES;
	currentpos += padding;
}

void print_formatted(char* s, char fmt){
		while(*s != 0){
		*(currentpos++) = *(s++);
		*(currentpos++) = fmt;
	} 
}

void print_positioned(char * s, char f, char c){
	char * cp = currentpos;
	currentpos = video + f*LINE_BYTES + c*2;
	print(s);
	currentpos = cp;
}

void clear(){
	currentpos = video;
	int c = 0;
	while(c < LINE_BYTES * COL_BYTES){
		*(currentpos++) = ' ';
		*(currentpos++) = 0x0f;
		c++;
	}
	currentpos = video;

}

void print(char * s){
	print_formatted(s, DEF_FORMAT);
}