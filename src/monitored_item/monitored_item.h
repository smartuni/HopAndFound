#ifndef MONITORED_ITEM_H
#define MONITORED_ITEM_H

#include <stdint.h>

#define UDP_RECV_PORT	((uint16_t) 8888)
#define MI_ID			(1)

void dispatch(uint8_t recv_buffer[]);

#endif /* MONITORED_ITEM_H */
