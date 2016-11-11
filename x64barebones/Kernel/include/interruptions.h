#ifndef _INTERRUPTIONS_H_
#define _INTERRUPTIONS_H_

#include <stdint.h>

void iSetHandler(int index, uint64_t handler);
void initInterruptions(void);

void sti();
void irq0Handler();
void irq1Handler();
void irq11Handler();


#endif
