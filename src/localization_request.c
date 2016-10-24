#include "localization.h"

void handle_localization_request(localization_request_t* p){
	localization_reply_t ret_pkg;
	ret_pkg.type = LOCALIZATION_REPLY;
	ret_pkg.node_id = 1;//TODO
	udp_send(&ret_pkg, sizeof(ret_pkg));
}

void send_localization_request(void){
	localization_request_t ret_pkg;
	ret_pkg.type = LOCALIZATION_REQUEST;
	udp_send(&ret_pkg, sizeof(ret_pkg));
}
