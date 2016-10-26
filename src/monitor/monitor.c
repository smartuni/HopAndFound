#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "thread.h"

#include "node.h"
#include "../HAF_protocol.h"

void dispatch(uint8_t recv_buffer[]) {
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
