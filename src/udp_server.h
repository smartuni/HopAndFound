#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <stdint.h>

typedef void (*dispatcher_callback_t)(uint8_t[]);

void udp_server(uint16_t port, dispatcher_callback_t cb);

#endif /* UDP_SERVER_H */
