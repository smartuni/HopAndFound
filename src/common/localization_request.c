#include "global.h"
#include "localization_request.h"
#include "call_for_help.h"
#include "localization_reply.h"
#include "xtimer.h"
#include "console_map.h"
#include "thread.h"

#define REQUEST_TIME_USEC (2000000)

//Priority of thread for handling task after waiting period for
//localization_replies ran out
#define LOCALIZATION_REQUEST_THREAD_PRIORITY THREAD_PRIORITY_MAIN + 3

//Stack for thread to handle task after _timer_localization_request interrupts	
char localization_request_stack[THREAD_STACKSIZE_MAIN];

//Callback function, which will be executed after waiting period for
//localization_replies is over
localization_request_cb_t callback_task;

xtimer_t _timer_localization_request;



/*
 * 	SENDING LOCALIZATION REQUEST
 */

void send_localization_request(void) {
	localization_request_t ret_pkg;
	ret_pkg.type = LOCALIZATION_REQUEST;
	udp_send(&ret_pkg, sizeof(ret_pkg), NULL);
	
#ifdef HAF_DEBUG
	printf("LOCALIZATION_REQUEST sent.\n");
#endif /* HAF_DEBUG */
	
	xtimer_set(&_timer_localization_request, REQUEST_TIME_USEC);
}

void _localization_request_handler(void){
	thread_create(localization_request_stack, sizeof(localization_request_stack),
			LOCALIZATION_REQUEST_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
			(void *) callback_task, NULL, "localization_request_callback");
}

void localization_request_init(localization_request_cb_t cb) {
	xtimer_init();
    _timer_localization_request.target = 0;
    _timer_localization_request.long_target = 0;
    _timer_localization_request.callback = (void*) _localization_request_handler;
    callback_task = cb;
}


/*
 * 	RECIEVING LOCALIZATION REQUEST
 */

void handle_localization_request(ipv6_addr_t* dst){
	send_localization_reply(dst);
}


/*
 * 	CALLBACK METHODS
 */

void localization_request_cb_monitored_item(void* arg) {
#ifdef HAF_DEBUG_NODE_MAP
	printConsoleMap(get_node_list(), MAX_NODES);
#endif /* HAF_DEBUG_NODE_MAP */

	send_call_for_help();
}

void localization_request_cb_node(void* arg) {
#ifdef HAF_DEBUG_NODE_MAP
	printConsoleMap(get_node_list(), MAX_NODES);
#endif /* HAF_DEBUG_NODE_MAP */

	resetNodeList();
}
