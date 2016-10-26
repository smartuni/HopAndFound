#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "thread.h"

#include "monitored_item.h"
#include "../HAF_protocol.h"
#include "heartbeat_handler.h"

void dispatch(uint8_t recv_buffer[]) {
	switch(recv_buffer[0]) {
		case HEARTBEAT:
			puts("------------------------------");
			puts("HEARTBEAT received.");
			heartbeat_t msg_strct_1;
			memcpy(&msg_strct_1, recv_buffer, sizeof(msg_strct_1));
			printf("type: %u\n", msg_strct_1.type);
			heartbeat_handler_start();
			break;
		case LOCALIZATION_REPLY:
			puts("------------------------------");
			puts("LOCALIZATION_REPLY received.");
			localization_reply_t msg_strct_2;
			memcpy(&msg_strct_2, recv_buffer, sizeof(msg_strct_2));
			printf("type: %u\n", msg_strct_2.type);
			printf("node_id: %u\n", msg_strct_2.node_id);
			//TODO call handler thread
			break;
		default:
			puts("------------------------------");
			puts("Irrelevant message received.");
			break;
	}
}
