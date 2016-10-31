#include <stdio.h>
#include <string.h>

#include "connection.h"
#include "xtimer.h"
#include "dispatcher.h"

int main(void){
	
	xtimer_sleep(1);
	set_netif(POWER, -21);
	set_netif(CHANNEL, 11);
	
	puts("Node start!");

	udp_server_start((dispatcher_callback_t)dispatch_node);
	
	puts("Node Server up");

	return 0;
}
