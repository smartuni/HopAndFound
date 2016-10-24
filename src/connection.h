#ifndef __CONNECTION__
#define __CONNECTION__

#include "net/gnrc/netapi.h"

#define UDP_RECV_PORT ((uint16_t) 9999) 	//Outer port for udp communication
#define UDP_SRC_PORT ((uint16_t) 1234)		//Inner port fpr udp communication
#define UDP_MULTICAST_ADDRESS ("ff02::1")	//IPv6 multicast address

/**
   Modes for set_netif()
*/
typedef enum netif_mode {
	POWER,
	CHANNEL
} netif_mode_t;

/**
  Packet structure which will be sent via multicast
*/
typedef struct packet{
	int header;
	int sequence_number;
	int message;
} packet_t;

/**
  Print content of packet p, additional text can be printed to identify packets
  \param info_text Additional info to identify packets on screen.
  \param p Pointer to packet_t struct which content will be printed.
*/
void print_packet(char* info_text, packet_t* p);

/**
  Send the content of p to multicast address 
  \param p Pointer to packet_t struct which content will be sent.
  \return The result of conn_udp_sendto().
*/
int udp_send(packet_t* p);

/**
  Start a udp server on port UDP_RECV_PORT
  \return -1 on error, 0 otherwise.
*/
int udp_server_start(void);

/**
  Set the CHANNEL or POWER (enum netif_mode) to val.
  \return 0 on success, 1 if the mode is invalid, 2 if val is negative in mode CHANNEL or 3 if no or more than 1 interface is available. If it is smaller than 0 it is the retval of gnrc_netapi_set(). 
*/
int set_netif(netif_mode_t mode, int16_t val);

#endif
