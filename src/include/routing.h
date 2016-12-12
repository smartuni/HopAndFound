#ifndef ROUTING_H
#define ROUTING_H

#include <stdint.h>

#include "global.h"
#include "HAF_protocol.h"

void init(void);
void _update(void);
void handle_update(update_t* p, uint32_t source_adr);
void check_exp(void);
bool checkroute(call_for_help_t* p);

#endif /* ROUTING_H */
