#ifndef __PCI_H
#define __PCI_H

void checkAllBuses(int from, int to);
void findRTL();
void initialize_device(uint8_t bus, uint8_t dev_func);
void deviceDetails(uint8_t bus, uint8_t dev);

#endif