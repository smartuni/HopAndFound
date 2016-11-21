#include "global.h"
#include "localization_request.h"
#include "call_for_help.h"
#include "localization_reply.h"
#include "xtimer.h"
#include "console_map.h"

#define REQUEST_TIME_USEC (2000000)

xtimer_t _timer;

void _localization_request_sender(void){	
	localization_request_t ret_pkg;
	ret_pkg.type = LOCALIZATION_REQUEST;
	udp_send(&ret_pkg, sizeof(ret_pkg), NULL);
	
#ifdef HAF_DEBUG
	printf("LOCALIZATION_REQUEST sent.\n");
#endif
	
	xtimer_set(&_timer, REQUEST_TIME_USEC);
}

void localization_request_init(localization_request_cb_t cb) {
	xtimer_init();
    _timer.target = 0;
    _timer.long_target = 0;
    _timer.callback = (void*)cb;
}

void handle_localization_request(ipv6_addr_t* dst){
	send_localization_reply(dst);
}

void send_localization_request(void) {
	_localization_request_sender();

}

void localization_request_cb_monitored_item(void* arg) {
#ifdef HAF_DEBUG_NODE_MAP
	printConsoleMap(get_node_list(), MAX_NODES);
#endif
	send_call_for_help();

	resetNodeList();
}

void localization_request_cb_node(void* arg) {
#ifdef HAF_DEBUG_NODE_MAP
	printConsoleMap(get_node_list(), MAX_NODES);
#endif

	resetNodeList();
}
