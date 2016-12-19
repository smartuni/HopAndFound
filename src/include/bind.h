#ifndef BIND_H
#define BIND_H
#include "HAF_protocol.h"
#include "connection.h"

void handle_bind(ipv6_addr_t* miIP);
void send_bind_ack(ipv6_addr_t* miIP);
void send_bind(void);

#endif /* BIND_H */
