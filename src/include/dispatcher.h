#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <stdint.h>

#include "net/ipv6/addr.h"

/* 
 * Type of dispatcher callback function.
 */
typedef void (*dispatcher_callback_t)(uint8_t[], ipv6_addr_t*);

/* 
 * Dispatch function for monitored item.
 */
void dispatch_monitored_item(uint8_t recv_buffer[], ipv6_addr_t* address);
/* 
 * Dispatch function for monitor.
 */
void dispatch_monitor(uint8_t recv_buffer[], ipv6_addr_t* address);
/* 
 * Dispatch function for node.
 */
void dispatch_node(uint8_t recv_buffer[], ipv6_addr_t* address);

#endif /* DISPATCHER_H */
