#include <stdio.h>
#include <string.h>

#include "global.h"
#include "xtimer.h"
#include "connection.h"
#include "dispatcher.h"
#include "haf_button.h"
#include "heartbeat.h"

int main(void){
	xtimer_sleep(STARTUP_SLEEPTIME_SEC);
	set_netif(POWER, SIGNAL_STRENGTH_MONITORED_ITEM);
	set_netif(CHANNEL, NETIF_CHANNEL);
#ifdef HAF_DEBUG
	puts("Monitored Item start!");
#endif
#ifdef TEST_PRESENTATION
	haf_button_init(haf_button_cb_monitored_item);
#endif
	udp_server_start((dispatcher_callback_t)dispatch_monitored_item);
	heartbeat_handler_init();
	
	return 0;
}
