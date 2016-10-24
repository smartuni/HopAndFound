#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "thread.h"

#include "HAF_protocol.h"
#include "udp_server.h"

#define UDP_RECV_PORT	((uint16_t) 8888)
#define M_ID			(1)

void dispatch(uint8_t recv_buffer[]) {
	switch(recv_buffer[0]) {
		case CALL_FOR_HELP: 
			printf("CALL_FOR_HELP received.");
			call_for_help_t msg_strct;
			memcpy(&msg_strct, recv_buffer, sizeof(msg_strct));
			//TODO call handler thread
			break;
		default:
			printf("Irrelevant message received.");
			break;
	}
}

int main(void) {
	udp_server(UDP_RECV_PORT, dispatch);
	return 0;
}
