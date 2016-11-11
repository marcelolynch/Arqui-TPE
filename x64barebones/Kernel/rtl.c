//http://wiki.osdev.org/RTL8139
#include <port.h>
#include <stdint.h>
#include <naiveConsole.h>

void * _memalloc(uint64_t size);

#define IOADDR 0xC000

#define TSAD0_OFFSET 0x20
#define TSD0_OFFSET 0x10 


static void* receiveBuffer;

void rtl_init(){

	// ===Turning on the RTL8139 ===
	//	Send 0x00 to the CONFIG_1 register (0x52) to set the LWAKE + LWPTN to active high.
	//	This should essentially power on the device. */

	sysOutByte( IOADDR + 0x52, 0x0); //Power on


// ==== Software Reset! =======
	// Next, we should do a software reset to clear the RX and TX buffers and set everything back to defaults.
	// Do this to eliminate the possibility of there still being garbage left in the buffers or registers on power on.
	// Sending 0x10 to the Command register (0x37) will send the RTL8139 into a software reset.
	// Once that byte is sent, the RST bit must be checked to make sure that the chip has finished the reset.
	// If the RST bit is high (1), then the reset is still in operation.

	// NB: There is a minor bug in Qemu. If you check the command register before performing a soft reset,
	// you may find the RST bit is high (1). Just ignore it and carry on with the initialization procedure.
	sysOutByte( IOADDR + 0x37, 0x10);
	//while( (sysInByte(IOADDR + 0x37) & 0x10) != 0) { /* nada */ }


	//Init Receive buffer

		//For this part, we will send the chip a memory location to use as its receive buffer start location.
		//One way to do it, would be to define a buffer variable and send that variables memory location

 	//ioaddr is obtained from PCI configuration
 	//sysOutLong(IOADDR + 0x30, (uintptr_t)rx_buffer);  send uint32_t memory location to RBSTART (0x30)
 	//to the RBSTART register (0x30).
	receiveBuffer = _memalloc(8*1024+16);
	sysOutLong(IOADDR + 0x30, (uint32_t)receiveBuffer);


 //===Set IMR + ISR=======
		// The Interrupt Mask Register (IMR) and Interrupt Service Register (ISR) are responsible
		// for firing up different IRQs. The IMR bits line up with the ISR bits to work in sync.
		// If an IMR bit is low, then the corresponding ISR bit with never fire an IRQ when the time comes for it to happen.
		// The IMR is located at 0x3C and the ISR is located at 0x3E.

		// To set the RTL8139 to accept only the Transmit OK (TOK) and Receive OK (ROK) interrupts,
		// we would have the TOK and ROK bits of the IMR high and leave the rest low.
		// That way when a TOK or ROK IRQ happens, it actually will go through and fire up an IRQ.
	sysOutWord(IOADDR + 0x3C, 0x0005); // Sets the TOK and ROK bits high


//  ====Configuring receive buffer (RCR)=====

// Before hoping to see a packet coming to you, you should tell the RTL8139 to accept packets based on various rules.
// The configuration register is RCR.

// You can enable different "matching" rules:

//     AB - Accept Broadcast: Accept broadcast packets sent to mac ff:ff:ff:ff:ff:ff
//     AM - Accept Multicast: Accept multicast packets.
//     APM - Accept Physical Match: Accept packets send to NIC's MAC address.
//     AAP - Accept All Packets. Accept all packets (run in promiscuous mode).

// Another bit, the WRAP bit, controls the handling of receive buffer wrap around.

// If WRAP is 0, the Rx buffer is treated as a traditional ring buffer:
	//if a packet is being written near the end of the buffer and the RTL8139 knows you've already handled
	//data before this (thanks to CAPR), the packet will continue at the beginning of the buffer.

// If WRAP is 1, the remainder of the packet will be written contiguously
	//(overflowing the actual receive buffer) so that it can be handled more efficiently.
	//This means the buffer must be an additional 1500 bytes (to hold the largest potentially overflowing packet).

// You can also tell the size of your RX buffer here, however if you use a 8k + 16
	//buffer as described before, writing zeroes is enough.
	//To use the WRAP=1 bit, an 8K buffer must in fact be 8k+16+1500 bytes.

	sysOutLong(IOADDR + 0x44, 0xf | (1 << 7)); // (1 << 7) is the WRAP bit, 0xf is AB+AM+APM+AAP


// ==== Enable Receive and Transmitter ====

// Now is the time to start up the RX and TX functions.
//  This is quite an easy piece, and should (in my opinion) only be done after all
//  of the configurations to the RTL8139's registers have been set to what is desired.
//  The RE (Receiver Enabled) and the TE (Transmitter Enabled) bits are located in the Command Register (0x37).
//  Starting up the RE and TE is pretty straight-forward, but lets go through it anyways.

// To enable the RTL8139 to accept and transmit packets, the RE and TE bits must go high.
// Once this is completed, then the card will start allowing packets in and/or out.

sysOutByte(IOADDR + 0x37, 0x0C); // Sets the RE and TE bits high
}


void rtlHandler(){
	// Aca habria que mirar que el bit 15(TOK) del descriptor sea 1, que indica que la transmision
	// de un packet fue exitosa, y hay que levantar la data que dejo en el receiveBuffer e imprimirla.
	// No se donde encontrar el descriptor. Tambien habria que mirar como hacer para enviar datos
	ncPrint("HOLA!");
	ncNewline();
}


void rtlPrintMac(){
	ncPrint("MAC: ");
	ncPrintHex(sysInByte(IOADDR));
	ncPrint(":");
	ncPrintHex(sysInByte(IOADDR + 1));
	ncPrint(":");
	ncPrintHex(sysInByte(IOADDR + 2));
	ncPrint(":");
	ncPrintHex(sysInByte(IOADDR + 3));	
	ncPrint(":");
	ncPrintHex(sysInByte(IOADDR + 4));	
	ncPrint(":");
	ncPrintHex(sysInByte(IOADDR + 5));	
}

/*
void memcpy(void * dst, void * src, uint32_t size){
	while(size--){
		char * dest = dst;
		char * source = src;
		*(dest++) = *(source++); 
	}
}*/


void rtlSend(){
	void * myMsg = _memalloc(1000);
	//char mac[] = {0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00};


}

