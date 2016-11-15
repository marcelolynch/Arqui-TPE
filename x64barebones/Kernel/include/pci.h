#ifndef __PCI_H
#define __PCI_H

void checkAllBuses(int from, int to);
void findRTL();
void dma_init();
void deviceDetails(uint8_t bus, uint8_t dev);

#endif