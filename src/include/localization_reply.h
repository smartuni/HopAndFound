#ifndef LOCALIZATION_REPLY_H
#define LOCALIZATION_REPLY_H

#include "HAF_protocol.h"
#include "connection.h"

void handle_localization_reply(localization_reply_t* p);
void send_localization_reply(ipv6_addr_t* dst);
uint8_t* get_node_list(void);
void resetNodeList(void);

#endif
