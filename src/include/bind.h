#ifndef BIND_H
#define BIND_H
#include "HAF_protocol.h"
#include "connection.h"

void handle_bind(bind_t* p);
void send_bind(void);

#endif /* BIND_H */
