#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <stdint.h>

void dispatch_monitored_item(uint8_t recv_buffer[]);
void dispatch_monitor(uint8_t recv_buffer[]);
void dispatch_node(uint8_t recv_buffer[]);

#endif /* DISPATCHER_H */
