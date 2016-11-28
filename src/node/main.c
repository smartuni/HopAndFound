#include <stdio.h>
#include <string.h>

#include "connection.h"
#include "xtimer.h"
#include "dispatcher.h"
#include "global.h"
#include "routing.h"

int main(void){
	
	xtimer_sleep(STARTUP_SLEEPTIME_SEC);
	set_netif(POWER,SIGNAL_STRENGTH_NODE);
	set_netif(CHANNEL, NETIF_CHANNEL);
	
	puts("Node start!");
	
	udp_server_start((dispatcher_callback_t)dispatch_node);
	init();
	
	return 0;
}
