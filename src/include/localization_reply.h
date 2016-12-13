#ifndef LOCALIZATION_REPLY_H
#define LOCALIZATION_REPLY_H

#include "HAF_protocol.h"

#ifdef HAF_USE_SOCK_UDP
#include "connection_sock.h"
#else
#include "connection.h"
#endif

void handle_localization_reply(localization_reply_t* p);
void send_localization_reply(ipv6_addr_t* dst);
uint8_t* get_node_list(void);
void resetNodeList(void);

#endif
