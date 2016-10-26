#include "monitor.h"
#include "udp_server.h"

int main(void) {
	udp_server(UDP_RECV_PORT, (void*)dispatch);
	return 0;
}
