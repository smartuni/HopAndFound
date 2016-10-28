#include "test_dispatcher.h"
#include "udp_server.h"
#include "heartbeat.h"

int main(void) {
	heartbeat_handler_init();
	udp_server(UDP_RECV_PORT, (void*)dispatch);
	return 0;
}
