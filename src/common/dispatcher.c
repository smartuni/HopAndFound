#include <stdio.h>
#include <string.h>

#include "thread.h"

#include "dispatcher.h"
#include "HAF_protocol.h"

void dispatch_monitored_item(uint8_t recv_buffer[]) {
	switch(recv_buffer[0]) {
		case HEARTBEAT:
			puts("------------------------------");
			puts("HEARTBEAT received.");
			heartbeat_t msg_strct_1;
			memcpy(&msg_strct_1, recv_buffer, sizeof(msg_strct_1));
			printf("type: %u\n", msg_strct_1.type);
			//TODO call handler thread
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

void dispatch_monitor(uint8_t recv_buffer[]) {
	switch(recv_buffer[0]) {
		case CALL_FOR_HELP:
			puts("------------------------------");
			puts("CALL_FOR_HELP received.");
			call_for_help_t msg_strct;
			memcpy(&msg_strct, recv_buffer, sizeof(msg_strct));
			printf("type: %u\n", msg_strct.type);
			printf("seq_nr: %lu\n", msg_strct.seq_nr);
			printf("mi_id: %u\n", msg_strct.mi_id);
			for(int i = 0; i < MAX_NODES; i++) {
				printf("node_list[%d]: %u\n", i, msg_strct.node_list[i]);
			}
			//TODO call handler thread
			break;
		default:
			puts("------------------------------");
			puts("Irrelevant message received.");
			break;
	}
}

void dispatch_node(uint8_t recv_buffer[]) {
	switch(recv_buffer[0]) {
		case LOCALIZATION_REQUEST:
			puts("------------------------------");
			puts("LOCALIZATION_REQUEST received.");
			localization_request_t msg_strct_1;
			memcpy(&msg_strct_1, recv_buffer, sizeof(msg_strct_1));
			printf("type: %u\n", msg_strct_1.type);
			//TODO call handler thread
			break;
		case CALL_FOR_HELP:
			puts("------------------------------");
			puts("CALL_FOR_HELP received.");
			call_for_help_t msg_strct_2;
			memcpy(&msg_strct_2, recv_buffer, sizeof(msg_strct_2));
			printf("type: %u\n", msg_strct_2.type);
			printf("seq_nr: %lu\n", msg_strct_2.seq_nr);
			printf("mi_id: %u\n", msg_strct_2.mi_id);
			for(int i = 0; i < MAX_NODES; i++) {
				printf("node_list[%d]: %u\n", i, msg_strct_2.node_list[i]);
			}
			//TODO call handler thread
			break;
		default:
			puts("------------------------------");
			puts("Irrelevant message received.");
			break;
	}
}
