#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "net/af.h"
#include "net/conn/udp.h"
#include "net/ipv6/addr.h"

#include "../../HAF_protocol.h"
#include "heartbeat.h"

#define UDP_MULTICAST_ADDRESS	("ff02::1")
#define UDP_SRC_PORT ((uint16_t) 8888)
#define UDP_RECV_PORT ((uint16_t) 8888)

int send_heartbeat(void) {
	int res;
	uint8_t send_buffer[sizeof(heartbeat_t)];
	heartbeat_t heartbeat;
	
	heartbeat.type = HEARTBEAT;
	
	memset(send_buffer, 0, sizeof(send_buffer));
	memcpy(send_buffer, &heartbeat, sizeof(heartbeat));
	
	
	ipv6_addr_t src = IPV6_ADDR_UNSPECIFIED, dst;
	if(ipv6_addr_from_str(&dst, UDP_MULTICAST_ADDRESS) == NULL) {
		return -1;
	}
	res = conn_udp_sendto(send_buffer, sizeof(send_buffer), &src, sizeof(src), 
							&dst, sizeof(dst), AF_INET6, UDP_SRC_PORT, UDP_RECV_PORT);
	return res;
}

int main(void) {
	heartbeat_sender_start();
	while(1) {
		
	}
	return 0;
}
