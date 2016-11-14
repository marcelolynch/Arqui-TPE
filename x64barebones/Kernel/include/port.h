#ifndef __PORT_H
#define __PORT_H
#include <stdint.h>

void sysOutLong( uint16_t addr, uint32_t data);
void sysOutWord( uint16_t addr, uint16_t data);
void sysOutByte( uint16_t addr, uint8_t data);

uint32_t sysInLong(uint16_t addr);
uint16_t sysInWord(uint16_t addr);
uint8_t sysInByte(uint16_t addr);

#endif