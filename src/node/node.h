#ifndef NODE_H
#define NODE_H

#include <stdint.h>

#define UDP_RECV_PORT	((uint16_t) 8888)
#define NODE_ID			(1)

void dispatch(uint8_t recv_buffer[]);

#endif /* NODE_H */
