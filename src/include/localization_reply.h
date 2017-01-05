#ifndef LOCALIZATION_REPLY_H
#define LOCALIZATION_REPLY_H

#include "HAF_protocol.h"
#include "global.h"
#include "connection.h"

/* 
 * Handles localization_reply.
 */
void handle_localization_reply(localization_reply_t* p);

/* 
 * sends localization_reply.
 */
void send_localization_reply(ipv6_addr_t* dst, ipv6_addr_t* monitor);

/* 
 *  returns nodelist.
 */
uint8_t* get_node_list(void);

/* 
 * resets nodelist.
 */
void resetNodeList(void);

/* 
 * clears routelist.
 */
void clear_route_list(void);


void get_route_p(ipv6_addr_t* ipv6_adr);
void get_hops_p(uint8_t* hops);

#endif
