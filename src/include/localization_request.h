#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include "HAF_protocol.h"
#include "connection.h"

void handle_localization_request(localization_request_t* p, ipv6_addr_t* dst);

void send_localization_request(void);

#endif
