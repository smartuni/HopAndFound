#ifndef LOCALIZATION_REPLY_H
#define LOCALIZATION_REPLY_H

#include "HAF_protocol.h"
#include "connection.h"

void handle_localization_reply(localization_reply_t* p);
void send_localization_reply(ipv6_addr_t* dst, ipv6_addr_t* monitor);
uint8_t* get_node_list(void);
void resetNodeList(void);
void clear_route_list(void);
void get_route_p(ipv6_addr_t* ipv6_adr);
void get_hops_p(uint8_t* hops);

#endif
