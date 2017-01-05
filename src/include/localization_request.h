#ifndef LOCALIZATION_REQUEST_H
#define LOCALIZATION_REQUEST_H

#include "HAF_protocol.h"
#include "global.h"
#include "connection.h"

typedef void (*localization_request_cb_t)(void);

/* 
 * Initializes localization_request.
 */
void localization_request_init(localization_request_cb_t cb);

/* 
 * Handles localization_request.
 */
void handle_localization_request(ipv6_addr_t* dst, ipv6_addr_t* monitor);

/* 
 * Sends localization_request.
 */
void send_localization_request(void);

/* 
 * Requests callbackfunction for localization_request.
 */
void localization_request_cb_monitored_item(void);

/* 
 * Requests callbackfunction for node
 */
void localization_request_cb_node(void);

#endif
