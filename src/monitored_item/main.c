#include "monitored_item.h"
#include "udp_server.h"
#include "heartbeat_handler.h"

int main(void) {
	heartbeat_handler_init();
	udp_server(UDP_RECV_PORT, (void*)dispatch);
	return 0;
}
