#include <stdio.h>
#include <string.h>

#include "connection.h"
#include "dispatcher.h"
#include "heartbeat.h"
#include "xtimer.h"

int main(void){
	
	xtimer_sleep(1);
	set_netif(POWER, -21);
	set_netif(CHANNEL, 11);
	
	puts("Monitor start!\n");

	udp_server_start((dispatcher_callback_t)dispatch_monitor);
	
	puts("Monitor Server up \n");
	
	heartbeat_sender_start();
	
	return 0;
}
