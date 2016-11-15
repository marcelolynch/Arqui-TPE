#ifndef __ETHERNET_H
#define __ETHERNET_H

/* IEEE 802.3 Ethernet magic constants. */
#define ETH_ALEN	6		/* #bytes in eth addr		*/
#define ETH_HLEN	14		/* #bytes in eth header		*/
#define ETH_ZLEN	60		/* min #bytes in frame		*/
#define ETH_FLEN	1536		/* max #bytes in frame		*/
#define ETH_DLEN	(ETH_FLEN - ETH_HLEN)	/* max #bytes of data	*/
/* These are the defined Ethernet Protocol ID's. */
#define ETH_P_LOOP	0x0060		/* Ethernet Loopback packet	*/
#define ETH_P_ECHO	0x0200		/* Ethernet Echo packet		*/
#define ETH_P_PUP	0x0400		/* Xerox PUP packet		*/
#define ETH_P_IP	0x0800		/* Internet Protocol packet	*/
#define ETH_P_ARP	0x0806		/* Address Resolution packet	*/
#define ETH_P_RARP      0x8035		/* Reverse Addr Res packet	*/
#define ETH_P_X25	0x0805		/* CCITT X.25			*/
#define ETH_P_IPX	0x8137		/* IPX over DIX			*/
#define ETH_P_802_3	0x0001		/* Dummy type for 802.3 frames  */
#define ETH_P_AX25	0x0002		/* Dummy protocol id for AX.25  */
#define ETH_P_ALL	0x0003		/* Every packet (be careful!!!) */
/* Define the Ethernet Broadcast Address (48 bits set to "1"). */
#define ETH_A_BCAST     "\377\377\377\377\377\377"
/* This is an Ethernet frame header. */


#pragma pack(push)
#pragma pack(1)


struct ethhdr {
  unsigned char		h_dest[ETH_ALEN];	/* destination eth addr	*/
  unsigned char		h_source[ETH_ALEN];	/* source ether addr	*/
  unsigned short	h_proto;		/* packet type ID field	*/
};
/* This is the complete Ethernet frame. */

typedef struct{
  struct ethhdr		f_hdr;			/* frame header		*/
  char			f_data[ETH_DLEN];	/* frame data (variable)*/
}  ethframe;

#pragma pack(pop)

#endif