#include <rtc.h>
#include <naiveConsole.h>

#define TIME_ZONE_DIF 3

void rtcInit(){
	sysOutByte(0x70, 0x0B);
	sysOutByte(0x71, 0x06);
}

uint64_t getSeconds(){
	sysOutByte(0x70, 0x00);
	return sysInByte(0x71);
}

uint64_t getMinutes(){
	sysOutByte(0x70, 0x02);
	return sysInByte(0x71);
}

uint64_t getHours(){
	sysOutByte(0x70, 0x04);
	return sysInByte(0x71) - TIME_ZONE_DIF;
}

uint64_t getWeekDay(){
	sysOutByte(0x70, 0x06);
	return sysInByte(0x71);

}

uint64_t getDayOfMonth(){
	sysOutByte(0x70, 0x07);
	return sysInByte(0x71);
}

uint64_t getMonth(){
	sysOutByte(0x70, 0x08);
	return sysInByte(0x71);

}

uint64_t getYear(){
	sysOutByte(0x70, 0x09);
	return sysInByte(0x71);

}

