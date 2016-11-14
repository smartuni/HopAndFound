#include <stdio.h>
#include <string.h>

#include "global.h"
#include "xtimer.h"
#include "connection.h"
#include "dispatcher.h"
#include "haf_button.h"
#include "haf_LED.h"

int main(void){
	xtimer_sleep(STARTUP_SLEEPTIME_SEC);
	set_netif(POWER,SIGNAL_STRENGTH_NODE);
	set_netif(CHANNEL, NETIF_CHANNEL);
#ifdef HAF_DEBUG
	puts("Node start!");
#endif
#ifdef TEST_PRESENTATION
	init_LED();
	haf_button_init(haf_button_cb_node);
#endif
	udp_server_start((dispatcher_callback_t)dispatch_node);
	
	return 0;
}
