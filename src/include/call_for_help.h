#ifndef CALL_FOR_HELP_H
#define CALL_FOR_HELP_H

#include "HAF_protocol.h"
#include "connection.h"

typedef enum handler{
	MONITOR,
	NODE,
	MONITORED_ITEM
} handler_t;

void handle_call_for_help(call_for_help_t* p, handler_t h);

void send_call_for_help(void);

void forward_call_for_help(call_for_help_t* p);

#endif
