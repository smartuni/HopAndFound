#ifndef HEARTBEAT_H
#define HEARTBEAT_H

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
void heartbeat_handler_init(void);
/* 
 * Handles incoming heartbeat.
 */
void handle_heartbeat(void);
/* 
 * Starts sending of heartbeats.
 */
void heartbeat_sender_start(void);
/* 
 * Stops sending of heartbeats.
 */
void heartbeat_sender_stop(void);

#endif /* HEARTBEAT_H */
