#include <stdio.h>
#include "localization_reply.h"
#include "global.h"
#include "routing.h"

#include <stdlib.h>

uint8_t node_list[MAX_NODES];
localization_reply_route_t route_list;

void send_localization_reply(ipv6_addr_t* dst, ipv6_addr_t* monitor){
	ipv6_addr_t ipv6_adr;
	localization_reply_t ret_pkg;
	ret_pkg.type = LOCALIZATION_REPLY;
	ret_pkg.node_id = NODE_ID;
	get_ipv6_addr(&ipv6_adr);
	ret_pkg.node_adr = ipv6_adr;

	ret_pkg.hops = get_route(monitor);

	udp_send(&ret_pkg, sizeof(ret_pkg), dst);
}

void handle_localization_reply(localization_reply_t* p){
	uint8_t node_id = p->node_id;

#ifdef HAF_DEBUG
	printf("LOC REPLY EMPFANGEN - NODE ADR: "); print_ipv6_string(&p->node_adr); printf("\n");
	printf("LOC REPLY EMPFANGEN - HOPS: %d\n", p->hops);
#endif

	if (node_id < 0 || node_id >= MAX_NODES){
#ifdef HAF_DEBUG
		printf("ERROR: received node id %d in handle_localization_reply\n", node_id);
#endif
		return;
	}

	if ( p->hops != 0 ) {
		if ( route_list.hops == 0 || p->hops < route_list.hops) {
#ifdef HAF_DEBUG
			printf("LOC REPLY ADRESSE HINZUGEFUEGT: "); print_ipv6_string(&p->node_adr); printf("\n");
#endif
			memcpy(&route_list.node_adr, &p->node_adr, sizeof(ipv6_addr_t)); //route_list.node_adr = p->node_adr;
		}
	}

	node_list[node_id] = 1;
}

uint8_t* get_node_list(void){
	return node_list;
}

void resetNodeList(void) {
	int i;
	for(i = 0; i < MAX_NODES; i++) {
		node_list[i] = 0;
	}
}

void clear_route_list(void) {
	route_list.hops = 0;
	 ipv6_addr_set_unspecified(&route_list.node_adr);
}

void get_route_p(ipv6_addr_t* ipv6_adr) {
	memcpy(ipv6_adr, &route_list.node_adr, sizeof(ipv6_addr_t));
}

void get_hops_p(uint8_t* hops) {
	memcpy(hops, &route_list.hops, sizeof(uint8_t));
}
