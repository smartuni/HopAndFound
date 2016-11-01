#include <stdio.h>
#include <string.h>

#include "connection.h"
#include "xtimer.h"
#include "dispatcher.h"
#include "heartbeat.h"
#include "global.h"

#include "call_for_help.h" 


int main(void){
	
	xtimer_sleep(STARTUP_SLEEPTIME_SEC);
	set_netif(POWER, SIGNAL_STRENGTH_MONITORED_ITEM);
	set_netif(CHANNEL, NETIF_CHANNEL);
	
	puts("Monitored Item start!");

	udp_server_start((dispatcher_callback_t)dispatch_monitored_item);
	
	heartbeat_handler_init();
	
	return 0;
}
