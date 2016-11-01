#ifndef LOCALIZATION_REQUEST_H
#define LOCALIZATION_REQUEST_H

#include "HAF_protocol.h"
#include "connection.h"

void handle_localization_request(ipv6_addr_t* dst);
void send_localization_request(void);

#endif
