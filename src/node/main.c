#include <stdio.h>
#include <string.h>

#include "global.h"
#include "xtimer.h"
#include "dispatcher.h"
#include "routing.h"
#include "haf_button.h"
#include "haf_LED.h"
#include "localization_request.h"

#ifdef HAF_USE_SOCK_UDP
#include "connection_sock.h"
#else
#include "connection.h"
#endif

int main(void){
	xtimer_sleep(STARTUP_SLEEPTIME_SEC);
	set_netif(POWER,SIGNAL_STRENGTH_NODE);
	set_netif(CHANNEL, NETIF_CHANNEL);

#ifdef HAF_DEBUG
	printf("Node start! ID: %d\n", NODE_ID);
#endif

	xtimer_init();
	init();

#ifdef TEST_PRESENTATION
	init_LED();
	haf_button_init(haf_button_cb_node);
	localization_request_init(localization_request_cb_node);
#endif

	udp_server_start((dispatcher_callback_t) dispatch_node);

	return 0;
}
