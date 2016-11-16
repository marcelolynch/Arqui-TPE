//http://wiki.osdev.org/RTL8139
#include <port.h>
#include <rtl.h>
#include <stdint.h>
#include <string.h>
#include <naiveConsole.h>
#include <ethernet.h>

void * _memalloc(uint64_t size);

#define IOADDR 0xC000

#define TSAD0 (IOADDR + 0x20)
#define TSAD1 (IOADDR + 0x24)
#define TSAD2 (IOADDR + 0x28)
#define TSAD3 (IOADDR + 0x2C)

#define TSD0 (IOADDR + 0x10)
#define TSD1 (IOADDR + 0x14)
#define TSD2 (IOADDR + 0x18)
#define TSD3 (IOADDR + 0x1C)

#define ISR (IOADDR + 0x3E) 


#define TX_SW_BUFFER_NUM 4
#define TSD_TOK (1 << 15)
#define TSD_OWN (1 << 13)


//Bitflags del ISR
#define TRANSMIT_OK  	(1 << 2)
#define RECEIVE_OK 		1
#define ISR_ERROR		(1<<1 | 1<<3)


#define BUF_SIZE 8*1024+16
#define MAC_SIZE 6
#define PROTO_SIZE 6
#define MSG_BUF_SIZE 100
#define MAX_MSG_SIZE 512

#define NETWORK_MAC "\xDE\xAD\xC0\xFF\xEE"


#define RX_HEADER_SIZE 4
#define RX_DATA_OFFSET (RX_HEADER_SIZE + ETH_HLEN) //Aca arranca la data posta en el frame ethernet 
											//(antes 4 bytes de header + 2 macs + 2 de proto)
#define USER_BYTE_OFFSET (RX_HEADER_SIZE + MAC_SIZE + MAC_SIZE - 1) //Ultimo byte de la MAC de origen


#define TRUE 1
#define FALSE 0


static int checkMAC(uint8_t* dir);


/*
	Este es el frame que se usa para 
*/
static struct {
	ethframe frame;
	uint32_t size;
} transmission;



static uint8_t receiveBuffer[BUF_SIZE] = {0};

/*
	Cada slot del buffer circular que guarda los mensajes recibidos
	tiene esta forma.
	
	El campo present indica si el slot esta ocupado 
	(si se guardo un mensaje y nunca se leyo) 

	El campo broadcast indica si fue un mensaje broadcast
	y user el numero de usuario (ultimo byte de la MAC de origen)
*/
typedef struct{
	char present;
	struct{
		char broadcast;
		char user;
		char data[MAX_MSG_SIZE + 1];
	} msg;
} msg_slot;



/*
	message_buffer guarda los mensajes y se implementa como
	un buffer circular: current apunta al proximo indice a escribir
	y pointer al proximo indice a leer. Si 
*/
static msg_slot message_buffer[MSG_BUF_SIZE];
static int pointer = 0;
static int current = 0;


static uint8_t currentDescriptor;


static void rtl_save_msg(int is_broadcast, char * msg);


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
	while( (sysInByte(IOADDR + 0x37) & 0x10) != 0) { /* nada */ }


	//Init Receive buffer

		//For this part, we will send the chip a memory location to use as its receive buffer start location.
		//One way to do it, would be to define a buffer variable and send that variables memory location

 	//ioaddr is obtained from PCI configuration
 	//sysOutLong(IOADDR + 0x30, (uintptr_t)rx_buffer);  send uint32_t memory location to RBSTART (0x30)
 	//to the RBSTART register (0x30).
	sysOutLong(IOADDR + 0x30, (uint32_t)receiveBuffer);


 //===Set IMR + ISR=======
		// The Interrupt Mask Register (IMR) and Interrupt Service Register (ISR) are responsible
		// for firing up different IRQs. The IMR bits line up with the ISR bits to work in sync.
		// If an IMR bit is low, then the corresponding ISR bit with never fire an IRQ when the time comes for it to happen.
		// The IMR is located at 0x3C and the ISR is located at 0x3E.

		// To set the RTL8139 to accept only the Transmit OK (TOK) and Receive OK (ROK) interrupts,
		// we would have the TOK and ROK bits of the IMR high and leave the rest low.
		// That way when a TOK or ROK IRQ happens, it actually will go through and fire up an IRQ.
	sysOutWord(IOADDR + 0x3C, 0x000f); // Sets the TOK and ROK bits high


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


	//Hago que todos los descriptores usen el mismo buffer
	//En realidad voy a usar uno solo
	sysOutLong(TSAD0, (uint32_t)&transmission.frame);
	sysOutLong(TSAD1, (uint32_t)&transmission.frame);
	sysOutLong(TSAD2, (uint32_t)&transmission.frame);
	sysOutLong(TSAD3, (uint32_t)&transmission.frame);

	//Seteo la MAC en el header del ethernet frame que vamos a usar
	int i;
	for(i=0; i < MAC_SIZE ; i++){
		transmission.frame.hdr.src[i] = sysInByte(IOADDR + i);
		
	}


	currentDescriptor = 0;
}



/*
	Handler de la interrupcion del RTL.
	El dispositivo interrumpe cuando termino de transmitir bien 
	con el bit TRANSMIT_OK en 1, y cuando termino de recibir con RECEIVE_OK en 1
	
	No estan activadas las interrupciones en caso de error, y en todo caso
	no se haría nada.

	En caso de estar terminando una transmision no se hace nada,
	si se recibio un paquete se llama a la rutina para guardar el mensaje
	en el buffer de mensajes.
*/
void rtlHandler(){
	int i;
	uint16_t isr = sysInWord(ISR);

	if(isr & TRANSMIT_OK){ 
		//Transmit OK - No hay que hacer nada
	}

	if(isr & RECEIVE_OK){

		if(checkMAC(receiveBuffer + RX_HEADER_SIZE + MAC_SIZE))
		{
			rtl_save_msg(1, receiveBuffer);
		}
	}

	rtl_init(); //Reseteo el dispositivo porque si no no anda
}


static int checkMAC(uint8_t* dir){
	return strncmp(NETWORK_MAC, dir, 5) == 0;
}





/*
	Esta funcion se llama cuando se recibe un mensaje nuevo.
	message_buffer es un buffer circular con los paquetes que van
	llegando. Cada slot del buffer indica en el campo "present" si
	esta ocupado o no (se desocupa cuando se pide con next_msg)

	Si el buffer esta lleno no se hace nada.

*/
static void rtl_save_msg(int is_broadcast, char * frame){	
	if(message_buffer[current].present == TRUE){
		return; //Buffer lleno
	}

	message_buffer[current].present = TRUE; //Ocupo el slot
	message_buffer[current].msg.broadcast = is_broadcast;
	message_buffer[current].msg.user = frame[USER_BYTE_OFFSET];

	/*ncPrint("Saving msg: "); ncPrint(msg);
	ncNewline();
	*/strncpy(message_buffer[current].msg.data, frame + RX_DATA_OFFSET, MAX_MSG_SIZE);

	current++;
	current = current % MSG_BUF_SIZE; //Volver al principio si se pasa
}



/*
	Se deja el mensaje no leido mas viejo en buf, copiando max_size caracteres
	como maximo  (o MAX_MSG_SIZE si este es mas chico). Lo que no se copia se pierde.
	
	Se devuelve si el mensaje era broadcast (1) o privado (0)

	Si no hay mensajes sin leer no se hace nada y se retorna -1.
*/

typedef struct{
	int broadcast;
	int user;
} msg_info;

int rtl_next_msg(char* buf, void * info, int max_size){



	if(message_buffer[pointer].present == FALSE){
		return -1; //No hay nada todavia
	}

	max_size = max_size < MAX_MSG_SIZE ? max_size : MAX_MSG_SIZE; //Escribo como maximo min(max_size, MAX_MSG_SIZE)

	char * next = message_buffer[pointer].msg.data;
	strncpy(buf, next, MAX_MSG_SIZE);

	message_buffer[pointer].present = FALSE; //Apago ese slot, ya lo lei
	

	((msg_info*)info)->broadcast = message_buffer[pointer].msg.broadcast;
	((msg_info*)info)->user = message_buffer[pointer].msg.user;

	pointer++;						//Avanzo en el buffer
	pointer = pointer%MSG_BUF_SIZE;

	return 0;
	
};



/*
Resetea el buffer de mensajes 
(vacia los slots y pone los punteros al principio)
*/
void rtl_clear_msgs(){
	int i;
	for(i=0; i< MAX_MSG_SIZE; i++){
		message_buffer[i].present = FALSE;
	}

	current = 0;
	pointer = 0;
}






/*
	Envia un mensaje (null terminated) a la MAC que se envia como parametro
	Se leen los primeros 6 bytes a partir de la mac destino. 

	Se usa el frame de Ethernet II (https://en.wikipedia.org/wiki/Ethernet_frame#Ethernet_II)
	Que tiene la estructura de ethframe en <ethernet.h>: 6 bytes de MAC destino, 6 bytes de
	MAC origen, 2 bytes de protipo (se pone el dummy type), y el resto de los bytes con el cuerpo
	del mensaje. 

	Mas info del mecanismo en la programmers guide (http://www.cs.usfca.edu/~cruse/cs326f04/RTL8139_ProgrammersGuide.pdf)
	y la data sheet para los registros (http://www.cs.usfca.edu/~cruse/cs326f04/RTL8139D_DataSheet.pdf)

*/
void rtl_send(char * msg, uint8_t * dst_mac){

	int i;

	//Broadcast
	for(i=0; i < MAC_SIZE ; i++){
		transmission.frame.hdr.dst[i] = '\xff';
	}

	uint32_t tsd = TSD0 + (currentDescriptor * 4);
	uint32_t tsad = TSAD0 + (currentDescriptor * 4);


	transmission.frame.hdr.proto = ETH_P_802_3; //Dummy type

	memcpy(transmission.frame.data, msg, strlen(msg));


	uint32_t descriptor = ETH_HLEN + strlen(msg); //Bits 0-12: Size
	transmission.size = descriptor;	
	descriptor &= ~(TSD_OWN); //Apago el bit 13 TSD_OWN
	descriptor &= ~(0x3f << 16);	// 21-16 threshold en 0
	
	while (!(sysInLong(tsd) & TSD_OWN))
		;

	sysOutLong(tsd, descriptor);
}






//LO QUE SIGUE ES PARA DEBUG

static int count = 0;
void _debug_rtl_handler(){
	ncClear();
	uint16_t isr = sysInWord(ISR);

	sysOutWord(ISR, 0x0);
	ncNewline();
	ncPrint("Interrupting with ISR: "); ncPrintHex(isr);
	ncPrint("  count: ");
	ncPrintDec(count++);
	ncNewline();


	ncClear(); 
	ncNewline();ncNewline();ncNewline();ncNewline();ncNewline();ncNewline();ncNewline();
	int i;
	ncPrint("ISR: "); ncPrintHex(isr); ncNewline();
	if(isr & TRANSMIT_OK){ //Transmit OK
	
		ncPrint("Transmitted ");
		ncPrintDec(transmission.size);
		ncPrint(" bytes.");
		ncNewline();

		ncPrint("Sent: ");
		uint8_t * buf = ((uint8_t*)(&transmission.frame));
		for(i = 0; i < 30 ; i++){
			ncPrintHex(buf[i]);
			ncPrint(" ");
		}

		ncNewline();

	}

	if(isr & RECEIVE_OK){
		ncPrint("Just recieved a package. It starts like this:");

		uint8_t * buf = ((uint8_t*)receiveBuffer);
		for(i = 0; i < 30 ; i++){
			ncPrintHex(buf[i]);
			ncPrint(" ");
		}

		ncNewline();

		rtl_save_msg(1, receiveBuffer + RX_DATA_OFFSET);
	}

	rtl_init(); //Reseteo el dispositivo porque si no no anda
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





void printDetails(char* msg){
	ncNewline();
	ncPrint(msg);
	ncPrint("   TSD0: 0x");
	ncPrintHex(sysInLong(TSD0));
	ncNewline();
	ncPrint("TSD1: 0x");
	ncPrintHex(sysInLong(TSD1));
	ncNewline();
	ncPrint("TSD2: 0x");
	ncPrintHex(sysInLong(TSD2));
	ncNewline();
	ncPrint("TSD3: 0x");
	ncPrintHex(sysInLong(TSD3));
	ncNewline();

}