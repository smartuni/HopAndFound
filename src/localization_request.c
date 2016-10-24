#include "localization.h"
#include "connection.h"

#define NODE_ID 1 //unique id to identify nodes

void handle_localization_request(localization_request_t* p, ipv6_addr_t dst){
	localization_reply_t ret_pkg;
	ret_pkg.type = LOCALIZATION_REPLY;
	ret_pkg.node_id = NODE_ID;
	udp_send(&ret_pkg, sizeof(ret_pkg), &dst);
}

void send_localization_request(void){
	localization_request_t ret_pkg;
	ret_pkg.type = LOCALIZATION_REQUEST;
	udp_send(&ret_pkg, sizeof(ret_pkg));
}
