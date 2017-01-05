#include "global.h"
#include "localization_request.h"
#include "call_for_help.h"
#include "localization_reply.h"
#include "xtimer.h"
#include "console_map.h"
#include "haf_queue.h"

#define REQUEST_TIME_USEC (2000000)


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
	memcpy(&ret_pkg.monitor, getMonitorIP(), sizeof(ipv6_addr_t));
	udp_send(&ret_pkg, sizeof(ret_pkg), NULL);

#ifdef HAF_DEBUG
	printf("LOCALIZATION_REQUEST sent.\n");
#endif /* HAF_DEBUG */

	xtimer_set(&_timer_localization_request, REQUEST_TIME_USEC);
}

void _localization_request_handler(void){
	if (haf_queue_enqueue((thread_function_t) callback_task) == -1){
#ifdef HAF_DEBUG
		printf("_localization_request_handler:: too many elements in thread queue.\n");
#endif /* HAF_DEBUG */
	}
}

void localization_request_init(localization_request_cb_t cb) {
    _timer_localization_request.target = 0;
    _timer_localization_request.long_target = 0;
    _timer_localization_request.callback = (void*) _localization_request_handler;
    callback_task = cb;
}


/*
 * 	RECIEVING LOCALIZATION REQUEST
 */
void handle_localization_request(ipv6_addr_t* dst, ipv6_addr_t* monitor){

	send_localization_reply(dst, monitor);
}


/*
 * 	CALLBACK METHODS
 */
void localization_request_cb_monitored_item(void) {
#ifdef HAF_DEBUG_NODE_MAP
	printConsoleMap(get_node_list(), MAX_NODES);
#endif /* HAF_DEBUG_NODE_MAP */

	send_call_for_help();
}

void localization_request_cb_node(void) {
#ifdef HAF_DEBUG_NODE_MAP
	printConsoleMap(get_node_list(), MAX_NODES);
#endif /* HAF_DEBUG_NODE_MAP */

	resetNodeList();
}
