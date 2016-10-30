#include "video_driver.h"

static char * video =  (char *)0xB8000;
static char * currentpos = (char *)0xB8000;
#define VCOLS 80
#define VROWS 25
#define LINE_BYTES 160
#define COL_BYTES 50
#define LAST_BYTE (video + VROWS * LINE_BYTES)
#define DEF_FORMAT 0x5f

void move_line(int from, int to){
	int i;
	char* f = video + from * LINE_BYTES;
	char* t = video + to * LINE_BYTES;
	for(i=0 ; i < LINE_BYTES ; i++){
		t[i] = f[i];
	}
}

void shift_screen_up(){
	int i;
	for(i = 0 ; i < VROWS; i++){
		move_line(i+1, i);
	}
	char* last_line = video + (VROWS-1)*LINE_BYTES;
	
	for(i = 0; i < LINE_BYTES ; i+=2){
		last_line[i] = ' ';
		last_line[i+1] = DEF_FORMAT;
	}
	
	currentpos -= LINE_BYTES;
}


void clear_line(){
	currentpos -= (currentpos-video)%LINE_BYTES;
}

void newline(){
	int padding = LINE_BYTES - (currentpos - video)%LINE_BYTES;
	currentpos += padding;

	while(currentpos >= LAST_BYTE){
		shift_screen_up();
	}
}


void screen_write(char* s, uint64_t size){
		while(size-- != 0){
		char next = *(s++);
		if(next == '\b'){
			currentpos -= 2;
			*currentpos = ' ';
		}else if (next == '\n'){
			newline();
		}else if(next == '\t'){
			int i;
			for(i = 0 ; i < 4 ; i++){
				*(currentpos++) = ' ';
				*(currentpos++) = DEF_FORMAT;
			}
		}
		else{
			*(currentpos++) = next;
			*(currentpos++) = DEF_FORMAT;
		}
		while(currentpos >= LAST_BYTE){
			shift_screen_up();
		}
	}
}


/*
void print_positioned(char * s, char f, char c){
	char * cp = currentpos;
	currentpos = video + f*LINE_BYTES + c*2;
	print(s);
	currentpos = cp;
}
*/
void clear_screen(){
	currentpos = video;
	int c = 0;
	while(c < LINE_BYTES * COL_BYTES){
		*(currentpos++) = ' ';
		*(currentpos++) = DEF_FORMAT;
		c++;
	}
	currentpos = video;

}

/*
void print(char * s){
	print_formatted(s, DEF_FORMAT);
}
*/
