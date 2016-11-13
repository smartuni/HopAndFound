#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <stdint.h>

#include "net/sock/udp.h"

typedef void (*dispatcher_callback_t)(uint8_t[], sock_udp_ep_t*);

void dispatch_monitored_item(uint8_t recv_buffer[], sock_udp_ep_t* address);
void dispatch_monitor(uint8_t recv_buffer[], sock_udp_ep_t* address);
void dispatch_node(uint8_t recv_buffer[], sock_udp_ep_t* address);

#endif /* DISPATCHER_H */
