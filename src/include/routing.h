#ifndef ROUTING_H
#define ROUTING_H

#include <stdint.h>

#include "global.h"
#include "HAF_protocol.h"

void init(void);
void _update(void);
void handle_update(update_t* p, ipv6_addr_t source_adr);
void check_exp(void);
bool checkroute(call_for_help_t* p);
void sendpkg(call_for_help_t* pkg);
uint8_t get_route(ipv6_addr_t* monitor); // gezieltest routing ab mi
#endif /* ROUTING_H */
