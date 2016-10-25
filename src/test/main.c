#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "net/af.h"
#include "net/conn/udp.h"
#include "net/ipv6/addr.h"
#include "thread.h"
#include "xtimer.h"

#include "../HAF_protocol.h"

#define UDP_MULTICAST_ADDRESS	("ff02::1")
#define UDP_SRC_PORT ((uint16_t) 8888)
#define UDP_RECV_PORT ((uint16_t) 8888)
#define SERVER_MSG_QUEUE_SIZE   (8)
#define MAX_SEND_BUFFER_SIZE	(12)

#define SLEEP_TIME				(2)

int send(uint8_t send_buffer[]) {
	int res;
	ipv6_addr_t src = IPV6_ADDR_UNSPECIFIED, dst;
	if(ipv6_addr_from_str(&dst, UDP_MULTICAST_ADDRESS) == NULL) {
		return -1;
	}
	res = conn_udp_sendto(send_buffer, sizeof(*send_buffer), &src, sizeof(src), 
							&dst, sizeof(dst), AF_INET6, UDP_SRC_PORT, UDP_RECV_PORT);
	return res;
}

int main(void) {
	puts("dispatcher_test");
	uint8_t send_buffer[MAX_SEND_BUFFER_SIZE];
	// declare packets
	heartbeat_t heartbeat;
	localization_request_t localization_request;
	localization_reply_t localization_reply;
	call_for_help_t call_for_help;
	
	//uint8_t node_list[MAX_NODES];
	
	// init packets
	heartbeat.type = HEARTBEAT;
	
	localization_request.type = LOCALIZATION_REQUEST;
	
	localization_reply.type = LOCALIZATION_REPLY;
	localization_reply.node_id = 1;
	
	call_for_help.type = CALL_FOR_HELP;
	call_for_help.seq_nr = 1;
	call_for_help.mi_id = 2;
	//call_for_help.node_list = node_list;
	
	while(1) {
		memcpy(send_buffer, &heartbeat, sizeof(heartbeat));
		send(send_buffer);
		puts("HEARTBEAT sent.");
		xtimer_sleep(SLEEP_TIME);
		
		memcpy(send_buffer, &localization_request, sizeof(localization_request));
		send(send_buffer);
		puts("LOCALIZATION_REQUEST sent.");
		xtimer_sleep(SLEEP_TIME);
		
		memcpy(send_buffer, &localization_reply, sizeof(localization_reply));
		send(send_buffer);
		puts("LOCALIZATION_REPLY sent.");
		xtimer_sleep(SLEEP_TIME);
		
		memcpy(send_buffer, &call_for_help, sizeof(call_for_help));
		send(send_buffer);
		puts("CALL_FOR_HELP sent.");
		xtimer_sleep(SLEEP_TIME);
	}
	return 0;
}
