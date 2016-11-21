static int TOTAL_TICKS = 0;


int getTicks(){
	return TOTAL_TICKS;
}


void tickHandler() {
	TOTAL_TICKS++;
}