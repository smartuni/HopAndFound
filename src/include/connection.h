#ifndef CONNECTION_H
#define CONNECTION_H

#include "net/gnrc/netapi.h"
#include "net/af.h"
#include "net/sock/udp.h"
#include "dispatcher.h"

#define UDP_RECV_PORT ((uint16_t) 8888) 	//Outer port for udp communication
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
  Send the content of p to multicast address 
  \param p Pointer to struct which content will be sent.
  \param p_size Size of struct.
  \param dst Pointer to destination address. May be NULL for multicast.
  \return The result of conn_udp_sendto().
*/
int udp_send(void* p, size_t p_size, sock_udp_ep_t* dst);

/**
  Start a udp server on port UDP_RECV_PORT
  \return -1 on error, 0 otherwise.
*/
int udp_server_start(dispatcher_callback_t cb);

/**
  Set the CHANNEL or POWER (enum netif_mode) to val.
  \return 0 on success, 1 if the mode is invalid, 2 if val is negative in mode CHANNEL or 3 if no or more than 1 interface is available. If it is smaller than 0 it is the retval of gnrc_netapi_set(). 
*/
int set_netif(netif_mode_t mode, int16_t val);

#endif
