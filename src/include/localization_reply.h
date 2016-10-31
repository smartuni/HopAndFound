#ifndef LOCALIZATION_REPLY_H
#define LOCALIZATION_REPLY_H

#include "HAF_protocol.h"
#include "connection.h"

void handle_localization_reply(localization_reply_t* p);

void send_localization_reply(void);

#endif
