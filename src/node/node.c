#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "thread.h"

#include "node.h"
#include "../HAF_protocol.h"

void dispatch(uint8_t recv_buffer[]) {
	switch(recv_buffer[0]) {
		case LOCALIZATION_REQUEST: 
			puts("LOCALIZATION_REQUEST received.");
			localization_request_t msg_strct_1;
			memcpy(&msg_strct_1, recv_buffer, sizeof(msg_strct_1));
			printf("type: %u\n", msg_strct_1.type);
			//TODO call handler thread
			break;
		case CALL_FOR_HELP:				
			puts("CALL_FOR_HELP received.");
			call_for_help_t msg_strct_2;
			memcpy(&msg_strct_2, recv_buffer, sizeof(msg_strct_2));
			printf("type: %u\n", msg_strct_2.type);
			printf("seq_nr: %lu\n", msg_strct_2.seq_nr);
			printf("mi_id: %u\n", msg_strct_2.mi_id);
			for(int i = 0; i < MAX_NODES; i++) {
				printf("node_list[%d]: %u\n", i, msg_strct_2.type);
			}
			//TODO call handler thread
			break;
		default:
			puts("Irrelevant message received.");
			break;
	}
}
