#ifndef MONITOR_H
#define MONITOR_H

#include <stdint.h>

#define UDP_RECV_PORT	((uint16_t) 8888)
#define M_ID			(1)

void dispatch(uint8_t recv_buffer[]);

#endif /* MONITOR_H */
