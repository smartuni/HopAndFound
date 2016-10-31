#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <stdint.h>

#include "net/ipv6/addr.h"

typedef void (*dispatcher_callback_t)(uint8_t[], ipv6_addr_t*);

void dispatch_monitored_item(uint8_t recv_buffer[], ipv6_addr_t* address);
void dispatch_monitor(uint8_t recv_buffer[], ipv6_addr_t* address);
void dispatch_node(uint8_t recv_buffer[], ipv6_addr_t* address);

#endif /* DISPATCHER_H */
