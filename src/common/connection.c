#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "thread.h"
#include "connection.h"
#include "HAF_protocol.h"
#include "xtimer.h"
#include "global.h"
#include "net/ipv6/addr.h"

#define SERVER_MSG_QUEUE_SIZE   (8)
#define MAX_RECV_BUFFER_SIZE	(sizeof(call_for_help_t))


static char stack[THREAD_STACKSIZE_DEFAULT];
static kernel_pid_t netif_dev = -1;
static sock_udp_t sock;


void* _udp_server(void *args){
	sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
	sock_udp_ep_t remote;
	
	uint8_t recv_buffer[MAX_RECV_BUFFER_SIZE];
	dispatcher_callback_t cb = (dispatcher_callback_t) args;
	
	local.port = UDP_RECV_PORT;
	
	if (sock_udp_create(&sock, &local, NULL, SOCK_FLAGS_REUSE_EP) < 0){
		puts("Error creating UDP sock");
		// TODO error handling
		return NULL;
	}
	
	while (1){
		if (sock_udp_recv(&sock, recv_buffer, sizeof(recv_buffer),
			SOCK_NO_TIMEOUT, &remote) < 0) {
			puts("Error receiving message");
			// TODO error handling
		} else {
			cb(recv_buffer, &remote);
		}
	}
}



int udp_send(void* p, size_t p_size, sock_udp_ep_t* dst){
	int res;
	
	if (dst != NULL){
		/*sock_udp_t _sock;
		sock_udp_create(&_sock, NULL, dst, SOCK_FLAGS_REUSE_EP);
		res = sock_udp_send(&_sock, p, p_size, NULL);
		sock_udp_close(&_sock);
		xtimer_usleep(1000);*/
		
		res = sock_udp_send(&sock, p, p_size, dst);
		xtimer_usleep(1000);
	} else {
		sock_udp_ep_t remote = {.family = AF_INET6,
								.port = UDP_RECV_PORT,
								.netif = SOCK_ADDR_ANY_NETIF};
									
		ipv6_addr_set_all_nodes_multicast((ipv6_addr_t *)&remote.addr.ipv6,
										  IPV6_ADDR_MCAST_SCP_LINK_LOCAL);
  
		res = sock_udp_send(&sock, p, p_size, &remote);
	}
	
#ifdef HAF_DEBUG
	if (res < 0){
		printf("SEND ERROR ERRVAL %d\n", res);
	}
#endif

	return res;
}

int udp_server_start(dispatcher_callback_t cb){
	if (thread_create(stack, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
					  THREAD_CREATE_STACKTEST, _udp_server, cb, "HopAndFound UDP Server")
		<= KERNEL_PID_UNDEF) {
		return -1;
	}

	return 0;
}

kernel_pid_t _get_netif(void){
	if (netif_dev != -1){
		return netif_dev;
	}
	
	kernel_pid_t ifs[GNRC_NETIF_NUMOF];
	size_t numof = gnrc_netif_get(ifs);
	
	if (numof == 1){
#ifdef HAF_DEBUG
		printf("IFID=%d\n", ifs[0]);
#endif
		netif_dev = ifs[0];
		return ifs[0];
	} else {
		return -1;
	}
}


int set_netif(netif_mode_t mode, int16_t val){
	int res;
	kernel_pid_t dev = _get_netif();
	
	if (dev == -1){
		return 3;
	}
	
	switch (mode){
		case POWER:
			res = gnrc_netapi_set(dev, NETOPT_TX_POWER, 0, (int16_t *)&val, sizeof(int16_t));
			break;
		case CHANNEL:
			if (val < 0){
				return 2;
			}
			res = gnrc_netapi_set(dev, NETOPT_CHANNEL, 0, (int16_t *)&val, sizeof(int16_t));
			break;
		default:
			return 1;	
	}
	
	return res < 0 ? res : 0;
}
