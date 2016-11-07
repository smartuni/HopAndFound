#include "localization_request.h"
#include "thread.h"
#include "call_for_help.h"
#include "localization_reply.h"
#include "xtimer.h"
#include "global.h"

#define REQUEST_SLEEP_TIME		(1)

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
