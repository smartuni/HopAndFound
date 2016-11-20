#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "thread.h"
#include "connection.h"
#include "HAF_protocol.h"
#include "xtimer.h"
#include "global.h"
#include "net/ipv6/addr.h"
#include "dispatcher.h"
#include "heartbeat.h"
#include "xtimer.h"
#include "global.h"

//#define SENDER

int main(void){
#ifndef SENDER
	uint8_t buf[5];
#else
	sock_udp_ep_t local = {	.family = AF_INET6,
							.port = UDP_RECV_PORT };
#endif
						
	sock_udp_t sock;	
	sock_udp_ep_t remote = { .family = AF_INET6,
						.port = UDP_RECV_PORT,
						.netif = SOCK_ADDR_ANY_NETIF};
						
	ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, MONITORED_ITEM_IP);

#ifdef SENDER
	sock_udp_create(&sock, &local, &remote, SOCK_FLAGS_REUSE_EP);
#else
	sock_udp_create(&sock, &remote, NULL, SOCK_FLAGS_REUSE_EP);
#endif
	
	while(1){
#ifdef SENDER
		sock_udp_send(&sock, "ABCD", sizeof("ABCD"), &remote);
		printf("Sent UDP Packet\n");
		xtimer_sleep(1);
#else
		sock_udp_recv(&sock, buf, sizeof(buf), SOCK_NO_TIMEOUT, &remote);
		printf("Recv UDP Packet: %s\n", buf);
#endif
	}
	
	return 0;
}
