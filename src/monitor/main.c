#include <stdio.h>
#include <string.h>

#include "connection.h"
#include "dispatcher.h"
#include "heartbeat.h"
#include "xtimer.h"
#include "global.h"

int main(void){
	
	xtimer_sleep(STARTUP_SLEEPTIME_SEC);
	set_netif(POWER, SIGNAL_STRENGTH_MONITOR);
	set_netif(CHANNEL, NETIF_CHANNEL);
	
	puts("Monitor start!");

	udp_server_start((dispatcher_callback_t)dispatch_monitor);
		
	heartbeat_sender_start();
	
	return 0;
}
