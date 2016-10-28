#include "test_dispatcher.h"
#include "udp_server.h"
#include "heartbeat.h"

int main(void) {
	udp_server(UDP_RECV_PORT, (void*)dispatch);
	heartbeat_handler_init();
	return 0;
}
