#ifndef __RTC_H
#define __RTC_H
#include <stdint.h>


void rtcInit();

uint64_t getSeconds();

uint64_t getMinutes();

uint64_t getHours();

uint64_t getWeekDay();

uint64_t getDayOfMonth();

uint64_t getMonth();

uint64_t getYear();

#endif