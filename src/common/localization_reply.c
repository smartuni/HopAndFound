#include <stdio.h>
#include "localization_reply.h"
#include "connection.h"
#include "global.h"

uint8_t node_list[MAX_NODES];
 
void send_localization_reply(ipv6_addr_t* dst){
	printf("send reply localization\n");
	localization_reply_t ret_pkg;
	ret_pkg.type = LOCALIZATION_REPLY;
	ret_pkg.node_id = NODE_ID;
	udp_send(&ret_pkg, sizeof(ret_pkg), dst);
}
 
void handle_localization_reply(localization_reply_t* p){
	uint8_t node_id = p->node_id;
	
	if (node_id < 0 || node_id >= MAX_NODES){
		printf("ERROR: received node id %d in handle_localization_reply\n", node_id);
		return;
	}
	
	node_list[node_id] = 1;
}

uint8_t* get_node_list(void){
	return node_list;
}
