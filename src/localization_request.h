#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include "HAF_protocol.h"

void handle_localization_request(localization_request_t* p);

void send_localization_request(void);

#endif
