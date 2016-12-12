#include <stdio.h>
#include <string.h>

#include "global.h"
#include "dispatcher.h"
#include "HAF_protocol.h"
#include "call_for_help.h"
#include "localization_reply.h"
#include "localization_request.h"
#include "heartbeat.h"
#include "routing.h"
#include "haf_LED.h"
#include "display.h"
#include "bind.h"

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
			printf("ttl: %u\n", call_for_help.ttl);
			printf("dest_adr: %u\n", call_for_help.dest_adr);
			for(int i = 0; i < MAX_NODES; i++) {
				printf("node_list[%d]: %u\n", i, call_for_help.node_list[i]);
			}
#endif
			
#ifdef TEST_PRESENTATION
			start_LED_blink(LED_RED, 3);
#endif /* TEST_PRESENTATION */

			handle_call_for_help(&call_for_help, MONITOR);
			
			break;
		}

		case BIND: {
			printf("Bind received \n");
			
			bind_t bind;
			memcpy(&bind, recv_buffer, sizeof(bind));
	
			handle_bind(&bind);
			break;
		}
		
		case UPDATE: {
			update_t update;
			uint8_t source_adr;
			memcpy(&update, recv_buffer, sizeof(update));
			source_adr = update.source_adr; //DATENTYP PRÜFEN
#ifdef HAF_DEBUG_DISPATCH
			puts("------------------------------");
			printf("UPDATE von MAC-Adr %d received.\n", source_adr);
			printf("type: %u\n", update.type);
			/*for(int i = 0; i < MAX_DEVICES; i++) {
				printf("routing_tbl[%d].mac_adr: %u\n", i, update.routing_tbl[i].mac_adr);
				printf("routing_tbl[%d].hops: %u\n", i, update.routing_tbl[i].hops);
				printf("routing_tbl[%d].next_hop_adr: %u\n", i, update.routing_tbl[i].next_hop_adr);
				printf("routing_tbl[%d].exp_time: %" PRIu32 "\n", i, update.routing_tbl[i].exp_time);
			}*/
#endif
			
			handle_update(&update, source_adr);
			

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
			
#ifdef TEST_PRESENTATION
			start_LED_blink(LED_BLUE, 3);
#endif /* TEST_PRESENTATION */
			
			handle_localization_request(address);
			
			break;
		}
#ifdef TEST_PRESENTATION
		case LOCALIZATION_REPLY: {
			localization_reply_t localization_reply;
			memcpy(&localization_reply, recv_buffer, sizeof(localization_reply));
#ifdef HAF_DEBUG_DISPATCH
			puts("------------------------------");
			puts("LOCALIZATION_REPLY received.");
			printf("type: %u\n", localization_reply.type);
			printf("node_id: %u\n", localization_reply.node_id);
#endif /* HAF_DEBUG_DISPATCH */
			uint8_t* node_list = get_node_list();
			if (node_list[localization_reply.node_id] == 1){
#ifdef HAF_DEBUG_DISPATCH
			printf("DROPPED\n");
#endif
				break;
			}
			
			handle_localization_reply(&localization_reply);
			
			break;
		}
#endif /* TEST_PRESENTATION */
		case CALL_FOR_HELP: {
			call_for_help_t call_for_help;
			memcpy(&call_for_help, recv_buffer, sizeof(call_for_help));
#ifdef HAF_DEBUG_DISPATCH
			puts("------------------------------");
			puts("CALL_FOR_HELP received.");
			printf("type: %u\n", call_for_help.type);
			printf("seq_nr: %lu\n", call_for_help.seq_nr);
			printf("mi_id: %u\n", call_for_help.mi_id);
			printf("ttl: %u\n", call_for_help.ttl);
			printf("dest_adr: %u\n", call_for_help.dest_adr);
			for(int i = 0; i < MAX_NODES; i++) {
				printf("node_list[%d]: %u\n", i, call_for_help.node_list[i]);
			}
#endif
			if ( checkroute(&call_for_help) ) handle_call_for_help(&call_for_help, NODE);
			
			break;
		}
		case UPDATE: {
			update_t update;
			uint8_t source_adr;
			memcpy(&update, recv_buffer, sizeof(update));
			source_adr = update.source_adr; //DATENTYP PRÜFEN
#ifdef HAF_DEBUG_DISPATCH
			puts("------------------------------");
			printf("UPDATE von MAC-Adr %d received.\n", source_adr);
			printf("type: %u\n", update.type);
			/*for(int i = 0; i < MAX_DEVICES; i++) {
				printf("routing_tbl[%d].mac_adr: %u\n", i, update.routing_tbl[i].mac_adr);
				printf("routing_tbl[%d].hops: %u\n", i, update.routing_tbl[i].hops);
				printf("routing_tbl[%d].next_hop_adr: %u\n", i, update.routing_tbl[i].next_hop_adr);
				printf("routing_tbl[%d].exp_time: %" PRIu32 "\n", i, update.routing_tbl[i].exp_time);
			}*/
#endif
			
			handle_update(&update, source_adr);
			
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
