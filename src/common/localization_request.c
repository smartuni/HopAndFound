#include "localization_request.h"
#include "thread.h"
#include "call_for_help.h"
#include "localization_reply.h"
#include "xtimer.h"

#define NODE_ID 1 //unique id to identify nodes
#define REQUEST_SLEEP_TIME		(1)

static char stack[THREAD_STACKSIZE_DEFAULT];

void handle_localization_request(ipv6_addr_t* dst){
	thread_create(stack, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, 
						_localization_request_handler, dst, "localization_request_handler");
}

void* _localization_request_handler(void* args){
	ipv6_addr_t dst_cpy;
	memcpy(&dst_cpy, (ipv6_addr_t*) args, sizeof(ipv6_addr_t));
	
	send_localization_reply(&dst_cpy);
	
	xtimer_sleep(REQUEST_SLEEP_TIME);
	
	send_call_for_help();
}

void send_localization_request(void){
	localization_request_t ret_pkg;
	ret_pkg.type = LOCALIZATION_REQUEST;
	udp_send(&ret_pkg, sizeof(ret_pkg), NULL);
}
