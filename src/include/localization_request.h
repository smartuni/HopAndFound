#ifndef LOCALIZATION_REQUEST_H
#define LOCALIZATION_REQUEST_H

#include "HAF_protocol.h"
#include "connection.h"

void localization_request_init(void);
void handle_localization_request(ipv6_addr_t* dst);
void send_localization_request(void);
void send_localization_request_node(void);

#endif
