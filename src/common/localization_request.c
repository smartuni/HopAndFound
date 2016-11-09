#include "global.h"
#include "localization_request.h"
#include "thread.h"
#include "call_for_help.h"
#include "localization_reply.h"
#include "xtimer.h"
#include "console_map.h"

#define REQUEST_SLEEP_TIME (2)

static char stack[THREAD_STACKSIZE_DEFAULT];

void* _localization_request_sender(void* args){	
	localization_request_t ret_pkg;
	ret_pkg.type = LOCALIZATION_REQUEST;
	udp_send(&ret_pkg, sizeof(ret_pkg), NULL);
	
	xtimer_sleep(REQUEST_SLEEP_TIME);
	
	send_call_for_help();
#ifdef HAF_DEBUG
	printf("CALL FOR HELP\n");
#endif
		
	return NULL;
}

void send_localization_request(void){
	thread_create(stack, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, 
						_localization_request_sender, NULL, "localization_request_sender");	
	
}

void handle_localization_request(ipv6_addr_t* dst){
	send_localization_reply(dst);
}

void* _localization_request_sender_node(void* args){	
	localization_request_t ret_pkg;
	ret_pkg.type = LOCALIZATION_REQUEST;
	udp_send(&ret_pkg, sizeof(ret_pkg), NULL);
	
	xtimer_sleep(REQUEST_SLEEP_TIME);
	
#ifdef HAF_DEBUG_NODE_MAP
	printConsoleMap(get_node_list(), MAX_NODES);
#endif

	resetNodeList();
		
	return NULL;
}

void send_localization_request_node(void) {
	thread_create(stack, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, 
						_localization_request_sender_node, NULL, "localization_request_sender");	
	
}
