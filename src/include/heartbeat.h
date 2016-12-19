#ifndef HEARTBEAT_H
#define HEARTBEAT_H
#include <net/gnrc/ipv6/netif.h>
/* 
 * Returns active state of heartbeat sending.
 */
bool getHeartbeatActive(void);
/* 
 * Sets active state of heartbeat sending.
 */
void setHeartbeatActive(bool status);

/* 
 * Initializes the heartbeat handler for incoming heartbeats.
 */
void heartbeat_timeout_init(void);
void heartbeat_timeout_start(void);
/* 
 * Handles incoming heartbeat.
 */
void handle_heartbeat(void);
/* 
 * Starts sending of heartbeats.
 */
void heartbeat_sender_init(ipv6_addr_t* miIP);
/* 
 * Stops sending of heartbeats.
 */
void heartbeat_sender_stop(void);

#endif /* HEARTBEAT_H */
