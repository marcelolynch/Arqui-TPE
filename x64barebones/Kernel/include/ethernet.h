//Fuente: http://www.scs.stanford.edu/histar/src/uinc/linux/if_ether.h

#ifndef __ETHERNET_H
#define __ETHERNET_H

/* IEEE 802.3 Ethernet magic constants. */
#define ETH_ALEN	6		/* #bytes in eth addr		*/
#define ETH_HLEN	14		/* #bytes in eth header		*/
#define ETH_ZLEN	60		/* min #bytes in frame		*/
#define ETH_FLEN	1536		/* max #bytes in frame		*/
#define ETH_DLEN	(ETH_FLEN - ETH_HLEN)	/* max #bytes of data	*/

#define ETH_P_802_3	0x0001		/* Dummy type for 802.3 frames  */

/* Define the Ethernet Broadcast Address (48 bits set to "1"). */
#define ETH_A_BCAST     "\377\377\377\377\377\377"



#pragma pack(push)
#pragma pack(1)

/* This is an Ethernet frame header. */
struct ethhdr {
  unsigned char		dst[ETH_ALEN];	/* destination eth addr	*/
  unsigned char		src[ETH_ALEN];	/* source ether addr	*/
  unsigned short	proto;		/* packet type ID field	*/
};


/* This is the complete Ethernet frame. */
typedef struct{
  struct ethhdr	hdr;			/* frame header		*/
  char			data[ETH_DLEN];	/* frame data (variable)*/
}  ethframe;

#pragma pack(pop)

#endif