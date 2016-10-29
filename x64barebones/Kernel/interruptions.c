//interruptions.c
#include <interruptions.h>
#include <keyboard_driver.h>

#pragma pack(push)
#pragma pack(1)

typedef struct { 
	uint16_t offset_l; //bit 0..15
	uint16_t selector;
	uint8_t zero_l;
	uint8_t attrs;
	uint16_t offset_m; //16..31
	uint32_t offset_h; //32..63
	uint32_t zero_h;
} IDTEntry_t;

#pragma pack(pop)

static IDTEntry_t* IDT = (IDTEntry_t*) 0x0;




void iSetHandler(int index, uint64_t handler) {
	IDT[index].offset_l = (uint16_t) handler & 0xFFFF;
	IDT[index].offset_m = (uint16_t) (handler >> 16) & 0xFFFF;
	IDT[index].offset_h = (uint32_t) (handler >> 32) & 0xFFFFFFFF;
	
	IDT[index].selector = 0x08;
	IDT[index].zero_l = 0;
	
	IDT[index].attrs = 0x8E;
	IDT[index].zero_h = 0;	
	
}



void tickHandler() {
}

void sti();
void irq0Handler();
void irq1Handler();


void syscallHandler();

void setPicMaster(uint16_t);

typedef void (*handler_t)(void);

handler_t handlers[] = {tickHandler, //IRQ0 - timer tick
						keyboardHandler,
						};

void irqDispatcher(int irq) {
	handlers[irq]();
}


	
void initInterruptions()
{	
	iSetHandler(0x20, (uint64_t) irq0Handler);
	iSetHandler(0x21, (uint64_t) irq1Handler);
//	iSetHandler(0x28, (uint64_t) irq8Handler);

	iSetHandler(0x80,(uint64_t)syscallHandler);
	//iSetHandler(0x)
	setPicMaster(0x7C); //0111 1100
	
	sti();
}
