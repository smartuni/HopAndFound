#ifndef HEARTBEAT_H
#define HEARTBEAT_H

void heartbeat_handler_init(void);
void handle_heartbeat(void);
void heartbeat_sender_start(void);
void heartbeat_sender_stop(void);

#endif /* HEARTBEAT_H */
