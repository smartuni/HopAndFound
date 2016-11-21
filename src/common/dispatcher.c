#include <stdio.h>
#include <string.h>

#include "global.h"
#include "dispatcher.h"
#include "HAF_protocol.h"
#include "call_for_help.h"
#include "localization_reply.h"
#include "localization_request.h"
#include "heartbeat.h"



void dispatch_monitored_item(uint8_t recv_buffer[], ipv6_addr_t* address) {
	switch(recv_buffer[0]) {
		case HEARTBEAT: {
			heartbeat_t heartbeat;
			memcpy(&heartbeat, recv_buffer, sizeof(heartbeat));
#ifdef HAF_DEBUG_DISPATCH
			puts("------------------------------");
			puts("HEARTBEAT received.");
			printf("type: %u\n", heartbeat.type);
#endif

			handle_heartbeat();
			
			break;
		}
		case LOCALIZATION_REPLY: {
			localization_reply_t localization_reply;
			memcpy(&localization_reply, recv_buffer, sizeof(localization_reply));
#ifdef HAF_DEBUG_DISPATCH
			puts("------------------------------");
			puts("LOCALIZATION_REPLY received.");
			printf("type: %u\n", localization_reply.type);
			printf("node_id: %u\n", localization_reply.node_id);
#endif
			
			handle_localization_reply(&localization_reply);
			
			break;
		}
		default: {
#ifdef HAF_DEBUG_DISPATCH
			puts("------------------------------");
			puts("Irrelevant message received.");
#endif
			break;
		}
	}
}

void dispatch_monitor(uint8_t recv_buffer[], ipv6_addr_t* address) {
	switch(recv_buffer[0]) {
		case CALL_FOR_HELP: {
			call_for_help_t call_for_help;
			memcpy(&call_for_help, recv_buffer, sizeof(call_for_help));
#ifdef HAF_DEBUG_DISPATCH
			puts("------------------------------");
			puts("CALL_FOR_HELP received.");
			printf("type: %u\n", call_for_help.type);
			printf("seq_nr: %lu\n", call_for_help.seq_nr);
			printf("mi_id: %u\n", call_for_help.mi_id);
			for(int i = 0; i < MAX_NODES; i++) {
				printf("node_list[%d]: %u\n", i, call_for_help.node_list[i]);
			}
#endif
			
			handle_call_for_help(&call_for_help, MONITOR);
			
			break;
		}
		default: {
#ifdef HAF_DEBUG_DISPATCH
			puts("------------------------------");
			puts("Irrelevant message received.");
#endif
			break;
		}
	}
}

void dispatch_node(uint8_t recv_buffer[], ipv6_addr_t* address) {
	switch(recv_buffer[0]) {
		case LOCALIZATION_REQUEST: {
			localization_request_t localization_request;
			memcpy(&localization_request, recv_buffer, sizeof(localization_request));
#ifdef HAF_DEBUG_DISPATCH
			puts("------------------------------");
			puts("LOCALIZATION_REQUEST received.");
			printf("type: %u\n", localization_request.type);
#endif
			
			handle_localization_request(address);
			
			break;
		}
		case CALL_FOR_HELP: {
			call_for_help_t call_for_help;
			memcpy(&call_for_help, recv_buffer, sizeof(call_for_help));
#ifdef HAF_DEBUG_DISPATCH
			puts("------------------------------");
			puts("CALL_FOR_HELP received.");
			printf("type: %u\n", call_for_help.type);
			printf("seq_nr: %lu\n", call_for_help.seq_nr);
			printf("mi_id: %u\n", call_for_help.mi_id);
			for(int i = 0; i < MAX_NODES; i++) {
				printf("node_list[%d]: %u\n", i, call_for_help.node_list[i]);
			}
#endif
			
			handle_call_for_help(&call_for_help, NODE);
			
			break;
		}
		default: {
#ifdef HAF_DEBUG_DISPATCH
			puts("------------------------------");
			puts("Irrelevant message received.");
#endif
			break;
		}
	}
}
