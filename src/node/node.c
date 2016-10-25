#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "thread.h"

#include "node.h"
#include "../HAF_protocol.h"

void dispatch(uint8_t recv_buffer[]) {
	switch(recv_buffer[0]) {
		case LOCALIZATION_REQUEST: 
			puts("LOCALIZATION_REQUEST received.\n");
			localization_request_t msg_strct_1;
			memcpy(&msg_strct_1, recv_buffer, sizeof(msg_strct_1));
			//TODO call handler thread
			break;
		case CALL_FOR_HELP:				
			puts("CALL_FOR_HELP received.\n");
			call_for_help_t msg_strct_2;
			memcpy(&msg_strct_2, recv_buffer, sizeof(msg_strct_2));
			//TODO call handler thread
			break;
		default:
			puts("Irrelevant message received.\n");
			break;
	}
}
